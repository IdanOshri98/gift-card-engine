#include <iostream>
#include "httplib.h"
#include "nlohmann/json.hpp"
#include "Wallet.h"
#include "FileRepository.h"
#include "RedemptionPlanner.h"
#include "RiskAnalyzer.h"

using json = nlohmann::json;

static json cardToJson(GiftCard& card) {
    card.updateDaysLeft();
    return {
        {"id",         card.getId()},
        {"title",      card.getTitle()},
        {"balance",    card.getBalance()},
        {"expiryDate", card.getExpiryDate()},
        {"daysLeft",   card.getDaysToExpiry()},
        {"companies",  card.getCompanies()}
    };
}

static json cardsToJson(std::vector<GiftCard>& cards) {
    json arr = json::array();
    for (GiftCard& card : cards) {
        arr.push_back(cardToJson(card));
    }
    return arr;
}

int main() {
    FileRepository repo("cards.txt");
    Wallet wallet;

    for (GiftCard& card : repo.load()) {
        wallet.addCard(card);
    }

    httplib::Server svr;

    // ── GET /api/cards ───────────────────────────────────────────────
    svr.Get("/api/cards", [&](const httplib::Request&, httplib::Response& res) {
        auto cards = wallet.getCards();
        res.set_content(cardsToJson(const_cast<std::vector<GiftCard>&>(cards)).dump(), "application/json");
    });

    // ── POST /api/cards ──────────────────────────────────────────────
    svr.Post("/api/cards", [&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body, nullptr, false);
        if (body.is_discarded()) {
            res.status = 400;
            res.set_content("{\"error\":\"Invalid JSON\"}", "application/json");
            return;
        }

        std::string title      = body.value("title", "");
        double balance         = body.value("balance", 0.0);
        std::string expiryDate = body.value("expiryDate", "");
        std::vector<std::string> companies = body.value("companies", std::vector<std::string>{});

        if (title.empty() || balance <= 0 || expiryDate.empty()) {
            res.status = 400;
            res.set_content("{\"error\":\"Missing or invalid fields\"}", "application/json");
            return;
        }

        int id = wallet.getNextId();
        GiftCard newCard(id, title, companies, balance, expiryDate);
        wallet.addCard(newCard);
        repo.save(wallet.getCards());

        res.status = 201;
        res.set_content(cardToJson(newCard).dump(), "application/json");
    });

    // ── DELETE /api/cards/:id ────────────────────────────────────────
    svr.Delete("/api/cards/(\\d+)", [&](const httplib::Request& req, httplib::Response& res) {
        int id = std::stoi(req.matches[1]);
        GiftCard* card = wallet.findCardById(id);
        if (card == nullptr) {
            res.status = 404;
            res.set_content("{\"error\":\"Card not found\"}", "application/json");
            return;
        }
        wallet.removeCard(*card);
        repo.save(wallet.getCards());
        res.set_content("{\"message\":\"Card removed\"}", "application/json");
    });

    // ── PUT /api/cards/:id ───────────────────────────────────────────
    svr.Put("/api/cards/(\\d+)", [&](const httplib::Request& req, httplib::Response& res) {
        int id = std::stoi(req.matches[1]);
        GiftCard* card = wallet.findCardById(id);
        if (card == nullptr) {
            res.status = 404;
            res.set_content("{\"error\":\"Card not found\"}", "application/json");
            return;
        }

        auto body = json::parse(req.body, nullptr, false);
        if (body.is_discarded()) {
            res.status = 400;
            res.set_content("{\"error\":\"Invalid JSON\"}", "application/json");
            return;
        }

        if (body.contains("title"))      card->setTitle(body["title"]);
        if (body.contains("balance"))    card->setBalance(body["balance"]);
        if (body.contains("expiryDate")) card->setExpiryDate(body["expiryDate"]);
        if (body.contains("companies")) {
            std::vector<std::string> newCompanies = body["companies"];
            wallet.syncCompaniesForCard(card, newCompanies);
        }

        repo.save(wallet.getCards());
        res.set_content(cardToJson(*card).dump(), "application/json");
    });

    // ── GET /api/cards/risk ──────────────────────────────────────────
    svr.Get("/api/cards/risk", [&](const httplib::Request&, httplib::Response& res) {
        auto cards   = wallet.getCards();
        auto atRisk  = RiskAnalyzer::getAtRiskCards(cards);
        double total = RiskAnalyzer::getTotalAtRiskBalance(cards);

        json response = {
            {"cards",        cardsToJson(atRisk)},
            {"totalBalance", total}
        };
        res.set_content(response.dump(), "application/json");
    });

    // ── GET /api/cards/plan ──────────────────────────────────────────
    svr.Get("/api/cards/plan", [&](const httplib::Request& req, httplib::Response& res) {
        std::string by = req.has_param("by") ? req.get_param_value("by") : "expiry";
        auto cards = wallet.getCards();

        std::vector<GiftCard> planned;
        if (by == "balance") {
            planned = RedemptionPlanner::planByBalance(cards);
        } else {
            planned = RedemptionPlanner::planByExpiry(cards);
        }

        res.set_content(cardsToJson(planned).dump(), "application/json");
    });

    // ── CORS headers for React ───────────────────────────────────────
    svr.set_pre_routing_handler([](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin",  "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        return httplib::Server::HandlerResponse::Unhandled;
    });

    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        res.status = 204;
    });

    std::cout << "Server running on http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}

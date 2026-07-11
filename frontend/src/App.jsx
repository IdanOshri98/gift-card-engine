import { useEffect, useState } from "react";
import CardList from "./components/CardList";
import AddCard from "./components/AddCard";
import EditCard from "./components/EditCard";
import { getCards, getRiskCards, getPlan } from "./api";
import "./App.css";

export default function App() {
  const [cards, setCards] = useState([]);
  const [view, setView] = useState("all");
  const [editingCard, setEditingCard] = useState(null);
  const [riskData, setRiskData] = useState(null);
  const [planCards, setPlanCards] = useState([]);

  async function loadCards() {
    const data = await getCards();
    setCards(data);
  }

  useEffect(() => {
    getCards().then(setCards);
  }, []);

  async function handleViewRisk() {
    const data = await getRiskCards();
    setRiskData(data);
    setView("risk");
  }

  async function handleViewPlan(by) {
    const data = await getPlan(by);
    setPlanCards(data);
    setView("plan");
  }

  return (
    <div className="app">
      <header>
        <h1>Gift Card Engine</h1>
        <nav>
          <button onClick={() => setView("all")}>All Cards</button>
          <button onClick={() => setView("add")}>+ Add Card</button>
          <button onClick={handleViewRisk}>Risk Analysis</button>
          <button onClick={() => handleViewPlan("expiry")}>Plan by Expiry</button>
          <button onClick={() => handleViewPlan("balance")}>Plan by Balance</button>
        </nav>
      </header>

      <main>
        {editingCard && (
          <EditCard
            card={editingCard}
            onSave={() => { setEditingCard(null); loadCards(); }}
            onCancel={() => setEditingCard(null)}
          />
        )}

        {!editingCard && view === "all" && (
          <>
            <h2>All Cards ({cards.length})</h2>
            <CardList
              cards={cards}
              onDelete={loadCards}
              onEdit={(card) => setEditingCard(card)}
            />
          </>
        )}

        {!editingCard && view === "add" && (
          <AddCard onAdd={() => { loadCards(); setView("all"); }} />
        )}

        {!editingCard && view === "risk" && riskData && (
          <>
            <h2>At-Risk Cards</h2>
            <p className="risk-total">
              Total at-risk balance: ₪{riskData.totalBalance.toFixed(2)}
            </p>
            <CardList
              cards={riskData.cards}
              onDelete={loadCards}
              onEdit={(card) => setEditingCard(card)}
            />
          </>
        )}

        {!editingCard && view === "plan" && (
          <>
            <h2>Redemption Plan</h2>
            <div className="plan-buttons">
              <button onClick={() => handleViewPlan("expiry")}>By Expiry</button>
              <button onClick={() => handleViewPlan("balance")}>By Balance</button>
            </div>
            <CardList
              cards={planCards}
              onDelete={loadCards}
              onEdit={(card) => setEditingCard(card)}
            />
          </>
        )}
      </main>
    </div>
  );
}

#include <gtest/gtest.h>
#include <fstream>
#include "FileRepository.h"
#include "GiftCard.h"

static const std::string TEST_FILE = "test_cards.txt";

static GiftCard makeCard(int id, double balance, const std::string& expiry,
                          const std::vector<std::string>& companies = {"Company"}) {
    std::string title = "Card" + std::to_string(id);
    std::vector<std::string> comp = companies;
    return GiftCard(id, title, comp, balance, expiry);
}

class FileRepositoryTest : public ::testing::Test {
protected:
    void TearDown() override {
        std::remove(TEST_FILE.c_str());
    }
};

TEST_F(FileRepositoryTest, SaveAndLoadSingleCard) {
    FileRepository repo(TEST_FILE);
    auto card = makeCard(1, 100.0, "01-01-2030", {"Amazon", "Fox"});

    repo.save({card});
    auto loaded = repo.load();

    ASSERT_EQ(loaded.size(), 1);
    EXPECT_EQ(loaded[0].getId(), 1);
    EXPECT_EQ(loaded[0].getTitle(), "Card1");
    EXPECT_DOUBLE_EQ(loaded[0].getBalance(), 100.0);
    EXPECT_EQ(loaded[0].getExpiryDate(), "01-01-2030");
    EXPECT_EQ(loaded[0].getCompanies().size(), 2);
}

TEST_F(FileRepositoryTest, SaveAndLoadMultipleCards) {
    FileRepository repo(TEST_FILE);
    auto c1 = makeCard(1, 100.0, "01-01-2030");
    auto c2 = makeCard(2, 250.0, "15-06-2026");

    repo.save({c1, c2});
    auto loaded = repo.load();

    ASSERT_EQ(loaded.size(), 2);
    EXPECT_EQ(loaded[0].getId(), 1);
    EXPECT_EQ(loaded[1].getId(), 2);
    EXPECT_DOUBLE_EQ(loaded[1].getBalance(), 250.0);
}

TEST_F(FileRepositoryTest, LoadFromMissingFileReturnsEmpty) {
    FileRepository repo("nonexistent_file.txt");
    auto loaded = repo.load();
    EXPECT_TRUE(loaded.empty());
}

TEST_F(FileRepositoryTest, SaveOverwritesPreviousData) {
    FileRepository repo(TEST_FILE);
    auto c1 = makeCard(1, 100.0, "01-01-2030");
    auto c2 = makeCard(2, 200.0, "01-01-2030");

    repo.save({c1});
    repo.save({c2});
    auto loaded = repo.load();

    ASSERT_EQ(loaded.size(), 1);
    EXPECT_EQ(loaded[0].getId(), 2);
}

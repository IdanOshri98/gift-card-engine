#ifndef FILEREPOSITORY_H
#define FILEREPOSITORY_H

#include "IRepository.h"
#include <string>

class FileRepository : public IRepository {
public:
    FileRepository(const std::string& filePath);

    void save(const std::vector<GiftCard>& cards) override;
    std::vector<GiftCard> load() override;

private:
    std::string filePath;
};

#endif

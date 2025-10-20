#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <string>
#include <iostream>
#include "mongo_utility.hpp"

class MongoWrapper {
private:
    mongocxx::client client;
    mongocxx::database db;
    mongocxx::collection collection;

public:
    // Constructor
    MongoWrapper(const std::string &uri, const std::string &db_name, const std::string &collection_name)
        : client(mongocxx::uri{uri}), db(client[db_name]), collection(db[collection_name]) {
        std::cout << "Connected to MongoDB database: " << db_name << ", collection: " << collection_name << "\n";
    }

    // Insert a document into the collection
    void insertDocument(const bsoncxx::document::view &doc) {
        try {
            collection.insert_one(doc);
            std::cout << "Document inserted into MongoDB.\n";
        } catch (const std::exception &e) {
            std::cerr << "Error inserting document: " << e.what() << "\n";
        }
    }

    // Find documents in the collection with a filter
    void findDocuments(const bsoncxx::document::view &filter) {
        try {
            auto cursor = collection.find(filter);
            for (const auto &doc : cursor) {
                std::cout << bsoncxx::to_json(doc) << "\n";
            }
        } catch (const std::exception &e) {
            std::cerr << "Error finding documents: " << e.what() << "\n";
        }
    }

    // Destructor
    ~MongoWrapper() {
        std::cout << "MongoDB connection closed.\n";
    }
};

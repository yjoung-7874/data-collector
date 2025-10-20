// #include "uploader/image_uploader.hpp"
#include "uploader/image_saver.hpp"

int main() {
//    const std::string mongo_uri = "mongodb://localhost:27017";
//    const std::string db_name = "realsense_db";
//    const std::string collection_name = "frames";

//    ImageUploader uploader(mongo_uri, db_name, collection_name);

//    uploader.startAndUpload(5, 1);
//
    RealsenseWrapper realsense;
    ImageSaver saver(realsense, "output_images");

    saver.startAndSave(1);

    return 0;
}

#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>
#include "../realsense/rs_wrapper.hpp"
#include "../mongodb/mongo_wrapper.hpp"

class ImageUploader {
private:
  RealsenseWrapper realsense;
  MongoWrapper mongo;

public:
  ImageUploader(const std::string &mongo_uri, const std::string &db_name, const std::string &collection_name)
      : mongo(mongo_uri, db_name, collection_name) {}

  void startAndUpload(int frame_count, int delay_seconds) {
    absl::Status status = realsense.start();
    if (!status.ok()) {
      std::cerr << "Failed to start Realsense pipeline: " << status.message() << "\n";
      return;
    }

    std::thread upload_thread([this, frame_count, delay_seconds]() {
      for (int i = 0; i < frame_count; ++i) {
          cv::Mat color_frame, depth_frame;
          absl::Status status = realsense.captureFrame(color_frame, depth_frame);

          if (status.ok()) {
              auto doc = matToBson(color_frame, depth_frame);
              mongo.insertDocument(doc.view());
              std::cout << "Frame " << i + 1 << " uploaded successfully.\n";
          } else {
              std::cerr << "Failed to capture frame " << i + 1 << ": " << status.message() << "\n";
          }

          std::this_thread::sleep_for(std::chrono::seconds(delay_seconds));
      }
    });

    upload_thread.join();
    realsense.stop();
  }
};

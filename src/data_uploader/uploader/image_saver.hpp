#include <thread>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <optional>
#include "../realsense/rs_wrapper.hpp"

class ImageSaver {
private:
  RealsenseWrapper& realsense;
  std::string save_directory;
  bool stop_requested;

public:
  ImageSaver(RealsenseWrapper& wrapper, const std::string& directory)
  : realsense(wrapper), save_directory(directory), stop_requested(false) {
    if (!std::filesystem::exists(save_directory)) {
      std::filesystem::create_directories(save_directory);
    }
  }

  void startAndSave(int delay_seconds, std::optional<int> frame_count = std::nullopt) {
    absl::Status status = realsense.start();
    if (!status.ok()) {
      std::cerr << "Failed to start Realsense pipeline: " << status.message() << "\n";
      return;
    }
    std::thread save_thread([this, frame_count, delay_seconds]() {
      int frame_index = 0;
      while (!stop_requested) {
        if (frame_count.has_value() && frame_index >= frame_count.value()) {
          break;
        }
	cv::Mat color_frame, depth_frame;
	absl::Status status = realsense.captureFrame(color_frame, depth_frame);

	if (status.ok()) {
          auto timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
 	  std::string time_str = std::to_string(timestamp);
	  std::string color_filename = save_directory + "/color_frame_" + time_str + ".png";
	  std::string depth_filename = save_directory + "/depth_frame_" + time_str + ".png";

	  bool color_saved = cv::imwrite(color_filename, color_frame);
	  bool depth_saved = cv::imwrite(depth_filename, depth_frame);
  
	  if (color_saved && depth_saved) {
            std::cout << "Frame " << frame_index + 1 << " saved successfully: "
	    << color_filename << ", " << depth_filename << "\n";
	  } else {
            std::cerr << "Failed to save frame " << frame_index + 1 << "\n";
	  }
	} else {
          std::cerr << "Failed to capture frame " << frame_index + 1 << ": " << status.message() << "\n";
	}

	frame_index++;
	std::this_thread::sleep_for(std::chrono::seconds(delay_seconds));
      }
    });
    save_thread.join();
    realsense.stop();
  }

  void stop() {
    stop_requested = true;
  }
};


#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>  
#include <absl/time/time.h>    
#include <absl/container/flat_hash_map.h>
#include <absl/status/status.h>
#include <iostream>
#include <string>

class RealsenseWrapper {
private:
    rs2::pipeline pipeline;      
    rs2::config config;          
    rs2::frameset frameset;      
    bool is_running;             

public:
  RealsenseWrapper() : is_running(false) {
    config.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    config.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30); 
  }

  absl::Status start() {
    try {
      pipeline.start(config);
      is_running = true;
      std::cout << "Realsense pipeline started successfully.\n";
      return absl::OkStatus();
    } catch (const rs2::error& e) {
      return absl::InternalError(std::string("Error starting pipeline: ") + e.what());
    }
  }

  absl::Status captureFrame(cv::Mat& color_frame, cv::Mat& depth_frame) {
    if (!is_running) {
        return absl::FailedPreconditionError("Pipeline is not running.");
    }

    try {
      frameset = pipeline.wait_for_frames();
      auto color = frameset.get_color_frame();
      if (color) {
        cv::Mat color_mat(cv::Size(640, 480), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
        color_frame = color_mat.clone();
      } else {
        return absl::NotFoundError("Failed to capture color frame.");
      }

      auto depth = frameset.get_depth_frame();
      if (depth) {
        cv::Mat depth_mat(cv::Size(640, 480), CV_16U, (void*)depth.get_data(), cv::Mat::AUTO_STEP);
        depth_frame = depth_mat.clone();
      } else {
        return absl::NotFoundError("Failed to capture depth frame.");
      }

      return absl::OkStatus();
    } catch (const rs2::error& e) {
      return absl::InternalError(std::string("Error capturing frame: ") + e.what());
    }
  }

  void saveFrame(const std::string& color_path, const std::string& depth_path) {
    cv::Mat color_frame, depth_frame;
    absl::Time start_time = absl::Now();

    absl::Status status = captureFrame(color_frame, depth_frame);
    if (status.ok()) {
      if (!color_frame.empty()) {
        cv::imwrite(color_path, color_frame);
        std::cout << "Saved color frame to: " << color_path << "\n";
      }
      if (!depth_frame.empty()) {
        cv::imwrite(depth_path, depth_frame);
        std::cout << "Saved depth frame to: " << depth_path << "\n";
      }
    } else {
      std::cerr << "Error: " << status.message() << "\n";
    }

    absl::Time end_time = absl::Now();
    std::cout << "Frame save duration: " << absl::ToDoubleMilliseconds(end_time - start_time) << " ms\n";
  }

  void stop() {
    if (is_running) {
      pipeline.stop();
      is_running = false;
      std::cout << "Realsense pipeline stopped.\n";
    }
  }

  ~RealsenseWrapper() {
    stop();
  }
};
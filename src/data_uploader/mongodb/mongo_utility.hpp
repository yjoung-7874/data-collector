#include <bsoncxx/builder/basic/document.hpp>

// Utility function to convert OpenCV Mat to BSON binary
bsoncxx::document::value matToBson(const cv::Mat &color_frame, const cv::Mat &depth_frame) {
  std::vector<uint8_t> color_data(color_frame.total() * color_frame.elemSize());
  memcpy(color_data.data(), color_frame.data, color_data.size());

  std::vector<uint8_t> depth_data(depth_frame.total() * depth_frame.elemSize());
  memcpy(depth_data.data(), depth_frame.data, depth_data.size());

  bsoncxx::builder::basic::document doc;
  doc.append(bsoncxx::builder::basic::kvp("color_frame", bsoncxx::types::b_binary{bsoncxx::binary_sub_type::k_binary, static_cast<uint32_t>(color_data.size()), color_data.data()}));
  doc.append(bsoncxx::builder::basic::kvp("depth_frame", bsoncxx::types::b_binary{bsoncxx::binary_sub_type::k_binary, static_cast<uint32_t>(depth_data.size()), depth_data.data()}));
  return doc.extract();
}
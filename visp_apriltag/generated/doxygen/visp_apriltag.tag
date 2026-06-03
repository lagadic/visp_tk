<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.8">
  <compound kind="class">
    <name>visp_apriltag::AprilTagTracker</name>
    <filename>classvisp__apriltag_1_1AprilTagTracker.html</filename>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>init_tracker</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>ae16a962bf0c75758160546c9caa1bf6d</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>init_info_strings</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>afab88e049d1a6abfe14b5df599bc94c0</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>image_callback</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>ab66b0683d7e484ac5e6e1ee74fa8c32a</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;msg)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Subscription&lt; sensor_msgs::msg::Image &gt;::SharedPtr</type>
      <name>m_rgb_stream_sub</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>a43f00297c3a03afc74dfa67120bbd1d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Publisher&lt; visp_tracker_common::msg::AprilTagDetectionArray &gt;::SharedPtr</type>
      <name>m_tags_info_pub</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>ab452a9d56c4f5d9ac05174186cd45197</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>vpDetectorAprilTag</type>
      <name>m_tag_detector</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>aba96561e4e37f74b1149872dd76b506d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_family_name</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>a8e72d4492a29b2f2cec117afa8655572</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::map&lt; int64_t, float &gt;</type>
      <name>m_tag_size_map</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>a9449654b452aa4654a0ae44d29f2f4fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>float</type>
      <name>m_default_tag_size</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>a73b9fc76114737b5cf55a0e0e4098ace</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>vpImage&lt; unsigned char &gt;</type>
      <name>m_I</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>a870657e5053ec5a2c627f383e28b7233</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::optional&lt; int &gt;</type>
      <name>m_opt_id</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>a128cb923526072c874e2afc27ad47e35</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>init_tracker</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>ae16a962bf0c75758160546c9caa1bf6d</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>init_info_strings</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>afab88e049d1a6abfe14b5df599bc94c0</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>image_callback</name>
      <anchorfile>classvisp__apriltag_1_1AprilTagTracker.html</anchorfile>
      <anchor>ab66b0683d7e484ac5e6e1ee74fa8c32a</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;msg)</arglist>
    </member>
  </compound>
</tagfile>

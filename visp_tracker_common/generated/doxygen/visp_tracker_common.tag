<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.8">
  <compound kind="class">
    <name>visp_tracker_common::BaseMultiModalTracker</name>
    <filename>classvisp__tracker__common_1_1BaseMultiModalTracker.html</filename>
    <base>visp_tracker_common::BaseTracker</base>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>init</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>aa5c83c92691e951a0c88e0cef2971be1</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>treat_rgb</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>ab9f7cd8a51546266495bae8b1b2fe8d3</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;rgb)=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>treat_depth</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>aa704d104ecd009796bbefd5e6b410d94</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;depth)=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>track</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a261bf71720f87f6c58db1320bc153f02</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>check_requires_depth</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>aac59cab8d53f65b4929615c4e0852c72</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>depth_camera_info_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a3e38402fb061e3bcceb627552805f855</anchor>
      <arglist>(const sensor_msgs::msg::CameraInfo::SharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>rgb_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>aa5b5ccfc9a08e015290bf21a19866780</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;rgb)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>sync_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>abab8e48e76358f97455d49a4d32fd7cd</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;rgb, const sensor_msgs::msg::Image::ConstSharedPtr &amp;depth)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Subscription&lt; sensor_msgs::msg::CameraInfo &gt;::SharedPtr</type>
      <name>m_depth_cam_info_sub</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a19bc629ae09c6809b58d0ce557dbd7f4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>message_filters::Subscriber&lt; sensor_msgs::msg::Image &gt;</type>
      <name>m_rgb_stream_sub</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a6291d4e633d2b522966a0a889661abd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>message_filters::Subscriber&lt; sensor_msgs::msg::Image &gt;</type>
      <name>m_depth_stream_sub</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>accfb8de0a1a276595a4ff7205ca0b3e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::shared_ptr&lt; message_filters::Synchronizer&lt; message_filters::sync_policies::ApproximateTime&lt; sensor_msgs::msg::Image, sensor_msgs::msg::Image &gt; &gt; &gt;</type>
      <name>m_streams_sync</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a97567812403d4049481260645b9a50a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Subscription&lt; sensor_msgs::msg::Image &gt;::SharedPtr</type>
      <name>m_rgb_only_stream_sub</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a2cb6c71048240591c604bb27881e20cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_depth_cam_info_received</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a8694cb4fff8fdac19099a60a719fc71a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_depth_camera_info_topic_name</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a1c187377dbdfc8fa4641ffdf86afbd03</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_depth_stream_name</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a793dc0423fb01e8677cd10841f5d168a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>VISP_NAMESPACE_ADDRESSING vpCameraParameters</type>
      <name>m_depth_cam</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a0c8d31ba49ce116fa8ea94ea1c1bf853</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_depth_is_required</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a5d4cfa6c785c7482348817d16c4d5167</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>m_z_factor</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a2425af3b39bc3658b8f95ce106be437c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_frame_id</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a0f3d32d7c030fcb708f8b7efd09caf8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>check_requires_depth</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>aac59cab8d53f65b4929615c4e0852c72</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>depth_camera_info_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>a3e38402fb061e3bcceb627552805f855</anchor>
      <arglist>(const sensor_msgs::msg::CameraInfo::SharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>rgb_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>aa5b5ccfc9a08e015290bf21a19866780</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;rgb)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>sync_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseMultiModalTracker.html</anchorfile>
      <anchor>abab8e48e76358f97455d49a4d32fd7cd</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;rgb, const sensor_msgs::msg::Image::ConstSharedPtr &amp;depth)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>visp_tracker_common::BaseTracker</name>
    <filename>classvisp__tracker__common_1_1BaseTracker.html</filename>
    <member kind="function">
      <type></type>
      <name>BaseTracker</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a0ed6ef4cbd098ca050757c9c1bbfce7e</anchor>
      <arglist>(const std::string &amp;node_name, const bool &amp;does_publish_features)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>init</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a786ad94483c04f0c0bd60b872b810653</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>has_to_quit</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6b825aa62b87d4fe03df83808a16849a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>stop_and_quit</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>abdf8048c41dd61112e2d5703ddca55fc</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="enumeration" protection="protected">
      <type></type>
      <name>InitializationMethod</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6ea659bd327687a484e630f8f70eef3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>CLICK</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6ea659bd327687a484e630f8f70eef3fa0e860a3de06962192ec746b62353f1eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>TOPIC</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6ea659bd327687a484e630f8f70eef3fa8320390804abc09f78289395c828bde8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>FILE</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6ea659bd327687a484e630f8f70eef3fad17d80e526ddc7f9248fe58769064ec8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>INITIALIZATION_METHOD_COUNT</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6ea659bd327687a484e630f8f70eef3fad9f53d49420e7116defdc9cf7b50c026</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef" protection="protected">
      <type>enum visp_tracker_common::BaseTracker::InitializationMethod</type>
      <name>InitializationMethod</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a99e59cce2748aabcc9b9c9c0d7cdf240</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>CLICK</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6ea659bd327687a484e630f8f70eef3fa0e860a3de06962192ec746b62353f1eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>TOPIC</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6ea659bd327687a484e630f8f70eef3fa8320390804abc09f78289395c828bde8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>FILE</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6ea659bd327687a484e630f8f70eef3fad17d80e526ddc7f9248fe58769064ec8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>INITIALIZATION_METHOD_COUNT</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6ea659bd327687a484e630f8f70eef3fad9f53d49420e7116defdc9cf7b50c026</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual bool</type>
      <name>init_tracker</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6a189d6a3aa47ac31c1142e00234f559</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>init_info_strings</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a3d628e39463fd27aa0c2612216fc323e</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>init_initialization_method</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>acf50c4c5fdef39ccf2f31895c4427e14</anchor>
      <arglist>(const rclcpp::Parameter &amp;p)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>color_camera_info_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a8f799ececfb5b8009a3dcba2079601f1</anchor>
      <arglist>(const sensor_msgs::msg::CameraInfo::SharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>init_pose_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a92557dd9cd27b4427076489938ac1457</anchor>
      <arglist>(const geometry_msgs::msg::PoseStamped::SharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>quit_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a1b54bdd3fcd13b6276e0af06428b2f96</anchor>
      <arglist>(const std::shared_ptr&lt; std_srvs::srv::Trigger::Request &gt; request, std::shared_ptr&lt; std_srvs::srv::Trigger::Response &gt; response)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>switch_tracking_status_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a13e2682afb2e4d7017e5d54d912d6c15</anchor>
      <arglist>(const std::shared_ptr&lt; std_srvs::srv::Trigger::Request &gt; request, std::shared_ptr&lt; std_srvs::srv::Trigger::Response &gt; response)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>switch_visual_status_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>adc4a97b5b5fe71d3dd16768d59b44b1c</anchor>
      <arglist>(const std::shared_ptr&lt; std_srvs::srv::Trigger::Request &gt; request, std::shared_ptr&lt; std_srvs::srv::Trigger::Response &gt; response)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static std::string</type>
      <name>initializationMethodToString</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>abe67f248b25a79394bbfae03039223e3</anchor>
      <arglist>(const InitializationMethod &amp;method)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static InitializationMethod</type>
      <name>initializationMethodFromString</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>ad2ef46ae0aab6b051023cbf9ab6368e7</anchor>
      <arglist>(const std::string &amp;name)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static std::string</type>
      <name>getAvailableInitializationMethod</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a7845c65b2ce5c0fc0d8f94e0b092e2c5</anchor>
      <arglist>(const std::string &amp;prefix=&quot;&lt; &quot;, const std::string &amp;sep=&quot; , &quot;, const std::string &amp;suffix=&quot; &gt;&quot;)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::shared_ptr&lt; rclcpp::ParameterEventHandler &gt;</type>
      <name>param_subscriber_</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>aa7542ecc3bc6b7aa131fd37831edf871</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::shared_ptr&lt; rclcpp::ParameterCallbackHandle &gt;</type>
      <name>cb_handle_</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>ace4bf1d81d7197558b3f67de67e04904</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::shared_ptr&lt; rclcpp::ParameterCallbackHandle &gt;</type>
      <name>cb_handle_init_</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a39d831fe8ab6cda72530b838fb2bc9a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Service&lt; std_srvs::srv::Trigger &gt;::SharedPtr</type>
      <name>m_quit_srv</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>af9fb3fdc5759916cc193c18c3683ce3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Service&lt; std_srvs::srv::Trigger &gt;::SharedPtr</type>
      <name>m_switch_tracking_status_srv</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a7b83d9e457476228a556cdb9b5526258</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Service&lt; std_srvs::srv::Trigger &gt;::SharedPtr</type>
      <name>m_switch_visual_srv</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a61170c6360c2a92dc53e4f637148636a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Subscription&lt; sensor_msgs::msg::CameraInfo &gt;::SharedPtr</type>
      <name>m_rgb_cam_info_sub</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>ac9f7dd0c473ef26fcbfd7e4231930236</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Subscription&lt; geometry_msgs::msg::PoseStamped &gt;::SharedPtr</type>
      <name>m_init_sub</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a4cd69f9f9ca3469c64e7ca877e2cef87</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::shared_ptr&lt; tf2_ros::TransformListener &gt;</type>
      <name>m_tf_listener</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>af66e4c3693048c6c4cd370692fb5c550</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::unique_ptr&lt; tf2_ros::Buffer &gt;</type>
      <name>m_tf_buffer</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a0da500e5d4abf70a7d759355117ea7a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Publisher&lt; visp_tracker_common::msg::NamedFeatureArray &gt;::SharedPtr</type>
      <name>m_features_pub</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>ad0281bc3c4a051e593e7cea7e7baf1c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Publisher&lt; visp_tracker_common::msg::InfoStrings &gt;::SharedPtr</type>
      <name>m_info_strings_pub</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>ab28d6e771f71cbb1868fd2789605c6ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Publisher&lt; geometry_msgs::msg::PoseStamped &gt;::SharedPtr</type>
      <name>m_poses_pub</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a991b68d7f04c76850c5720e09f338a23</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_is_headless_mode</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a9fd3602793e36d481a3c944e7b546c02</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>m_display_nb_frames_skipped</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>ae561e01a5bc054a0ba1510ab61a6dd0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>unsigned int</type>
      <name>m_frame_cnt</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a84633646ed112b6a10363c80d285d9f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>visp_tracker_common::msg::InfoStrings</type>
      <name>m_info_strings</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>ac3b18a7739e1df1eff8fddf9434d0f29</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>unsigned int</type>
      <name>m_info_nb_static</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a411a6a7c29929d0d1462ce74726beea6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_visualization_debug</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>aa90fe323829cecfb8061edbd4051a997</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_rgb_cam_info_received</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a82012a3da12da0a98764bcba5da18418</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_rgb_camera_info_topic_name</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>ab1eae20fda6e63de32f2aae05948fd03</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_rgb_stream_name</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>ac191581077e0eb166da5d78e300bd70c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>VISP_NAMESPACE_ADDRESSING vpCameraParameters</type>
      <name>m_rgb_cam</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a0f27d9d04c4597e5a5d750b50a5fc93c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_config_file</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a76560cf4b94a892db64eceb4f3879edd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>InitializationMethod</type>
      <name>m_init_method</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>acb2fdfb133b94102106297c0a9e63f43</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::optional&lt; geometry_msgs::msg::PoseStamped &gt;</type>
      <name>m_opt_init_pose</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a17c413422c4fa11e7e40d3c8233b3eb9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::mutex</type>
      <name>m_mutex_quit</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6bc4f006d01618011351fa877fd747d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_quit</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a9324ac8364a7ef7c81327ca4942144da</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::mutex</type>
      <name>m_mutex_tracking</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>abf53cddde9e1897bcbc888aa9fb957a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_has_to_track</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>aa371c209a777603a62a45974d4b2b26e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::mutex</type>
      <name>m_mutex_visualization</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a70021e81905c550c4c9d66f52f33da40</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static const std::string</type>
      <name>s_dumb_topic_name</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a68eea5dbf94cf618d8d1b9c9850741b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static const unsigned int</type>
      <name>s_default_hor_offset</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a59fb6fa95492234b32e368f8a3474e5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected" static="yes">
      <type>static const unsigned int</type>
      <name>s_hor_offset_from_left_border</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>af5860fc828aeb53a8c81eb997c10579a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual bool</type>
      <name>init_tracker</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a6a189d6a3aa47ac31c1142e00234f559</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="pure">
      <type>virtual void</type>
      <name>init_info_strings</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a3d628e39463fd27aa0c2612216fc323e</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>init_initialization_method</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>acf50c4c5fdef39ccf2f31895c4427e14</anchor>
      <arglist>(const rclcpp::Parameter &amp;p)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>color_camera_info_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a8f799ececfb5b8009a3dcba2079601f1</anchor>
      <arglist>(const sensor_msgs::msg::CameraInfo::SharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>init_pose_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a92557dd9cd27b4427076489938ac1457</anchor>
      <arglist>(const geometry_msgs::msg::PoseStamped::SharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>quit_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a1b54bdd3fcd13b6276e0af06428b2f96</anchor>
      <arglist>(const std::shared_ptr&lt; std_srvs::srv::Trigger::Request &gt; request, std::shared_ptr&lt; std_srvs::srv::Trigger::Response &gt; response)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>switch_tracking_status_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>a13e2682afb2e4d7017e5d54d912d6c15</anchor>
      <arglist>(const std::shared_ptr&lt; std_srvs::srv::Trigger::Request &gt; request, std::shared_ptr&lt; std_srvs::srv::Trigger::Response &gt; response)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>switch_visual_status_callback</name>
      <anchorfile>classvisp__tracker__common_1_1BaseTracker.html</anchorfile>
      <anchor>adc4a97b5b5fe71d3dd16768d59b44b1c</anchor>
      <arglist>(const std::shared_ptr&lt; std_srvs::srv::Trigger::Request &gt; request, std::shared_ptr&lt; std_srvs::srv::Trigger::Response &gt; response)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>visp_tracker_common::TrackerGUI</name>
    <filename>classvisp__tracker__common_1_1TrackerGUI.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>init</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a83378a186a567c87868bf19647fce2a1</anchor>
      <arglist>(std::shared_ptr&lt; rclcpp::Node &gt; self)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>has_to_run</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a349e51ebb1c0505101698e419c9f8880</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>quit</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a65d1bfbcc6eb50ef85057677918eee32</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>POINT</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a7c196dd620fac2496c38a1f84b6e8f98aefbfa9f1075046370c42dd4de721496a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>CROSS</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a7c196dd620fac2496c38a1f84b6e8f98a1608db77b8b911a3aa3aefc77df1af49</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>camera_info_callback</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a61830266b6cb5a3ad562aa46fe7da752</anchor>
      <arglist>(const sensor_msgs::msg::CameraInfo::SharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>image_callback</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>af233bb3925d57e00a06dd95ef3a20b27</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>depth_callback</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a196ba4e2c609d93c0c11863a9f17560e</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>info_callback</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>aefd684f4a3021fe4a376a68baa3ba0fa</anchor>
      <arglist>(const visp_tracker_common::msg::InfoStrings::ConstSharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>switch_tracking</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a0f0726865c764a021f8cd3ed7e9e067d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>switch_visualization</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a677f3dc6d8c70c7968f6cbbd4bf9768d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Node::SharedPtr</type>
      <name>m_service_node</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>aac0f0c719b449ec3cf90c0a57b049d3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std_srvs::srv::Trigger::Request::SharedPtr</type>
      <name>m_quit_request</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a165b306617bd28efe21711ec08d2e961</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::shared_ptr&lt; rclcpp::Client&lt; std_srvs::srv::Trigger &gt; &gt; &gt;</type>
      <name>m_clients_quit</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>ac455b89006efa8d0583755821d242e74</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std_srvs::srv::Trigger::Request::SharedPtr</type>
      <name>m_switch_request</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a06f5b93c17ecc09ac8d006064479e09c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::shared_ptr&lt; rclcpp::Client&lt; std_srvs::srv::Trigger &gt; &gt; &gt;</type>
      <name>m_clients_switch_tracking</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a0f1114ad3c64503e5e890c4ccad9ca95</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::shared_ptr&lt; rclcpp::Client&lt; std_srvs::srv::Trigger &gt; &gt; &gt;</type>
      <name>m_clients_switch_visualization</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a5dc47902ef35d3f13844092f9cfc9244</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Subscription&lt; sensor_msgs::msg::CameraInfo &gt;::SharedPtr</type>
      <name>m_rgb_cam_sub</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a81b6608ca9699203d1803a3eb750f0d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; rclcpp::Subscription&lt; visp_tracker_common::msg::NamedFeatureArray &gt;::SharedPtr &gt;</type>
      <name>m_feat_2D_sub</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a0f6a4337a657bce7473b6b2691ba095b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>rclcpp::Subscription&lt; visp_tracker_common::msg::InfoStrings &gt;::SharedPtr</type>
      <name>m_info_strings_sub</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a7ca6e4f759e369628ce9484eb474e174</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; rclcpp::Subscription&lt; geometry_msgs::msg::PoseStamped &gt;::SharedPtr &gt;</type>
      <name>m_poses_sub</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a61468f56e9c0f46a8539a8044f3099a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::optional&lt; visp_tracker_common::msg::NamedFeatureArray &gt; &gt;</type>
      <name>m_feature_opt_vec</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a0c0a79bdfb63c75db1a52d03d9354eb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::mutex</type>
      <name>m_mutex_features</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a1122cd6bc99539c611bdac59404a26e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::optional&lt; geometry_msgs::msg::PoseStamped &gt; &gt;</type>
      <name>m_pose_opt_vec</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a671c127d794f7f13dc7d0104ea094d2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::string &gt;</type>
      <name>m_pose_name_array</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a03452266e3fad1802828a79138a8aeb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::mutex</type>
      <name>m_mutex_poses</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a983e0ca80f9d9b8f5ae6ffc75314ef55</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>visp_tracker_common::msg::InfoStrings</type>
      <name>m_vec_info</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a39a1ac95bcd143ad99ab5098a515ecd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::mutex</type>
      <name>m_mutex_info</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a562e23ddc5744c9778ca19d1d515c6fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; rclcpp::Subscription&lt; visp_tracker_common::msg::AprilTagDetectionArray &gt;::SharedPtr &gt;</type>
      <name>m_apriltag_sub</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a6a506b7ef2386513433a8d8d5fe2d29b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::optional&lt; visp_tracker_common::msg::AprilTagDetectionArray &gt; &gt;</type>
      <name>m_apriltag_opt_vec</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>ad6f27ae43d1cfe029e0c3eb5ffce1f83</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::mutex</type>
      <name>m_mutex_apriltag</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>ac9c06d796acf3ddede3584877211e010</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_use_depth</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>ac0d8e683bc2260025d5cf37407bda656</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::shared_ptr&lt; VISP_NAMESPACE_ADDRESSING vpDisplay &gt;</type>
      <name>m_display_color</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a2daec64b7980f62f3e958e07b32d5132</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::shared_ptr&lt; VISP_NAMESPACE_ADDRESSING vpDisplay &gt;</type>
      <name>m_display_depth</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>aeaef65a944bc6283b8d2967dd74b30e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>VISP_NAMESPACE_ADDRESSING vpImage&lt; VISP_NAMESPACE_ADDRESSING vpRGBa &gt;</type>
      <name>m_I</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>acab4dc67380baa7f4230d94c94ba9c99</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::mutex</type>
      <name>m_mutex_Id</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>aeae742498acdcaceae4f9b011d186fd5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::optional&lt; VISP_NAMESPACE_ADDRESSING vpImage&lt; VISP_NAMESPACE_ADDRESSING vpRGBa &gt; &gt;</type>
      <name>m_opt_Id</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a786194ef32b486e0a9cd662cbe84c9b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::optional&lt; uint16_t &gt;</type>
      <name>m_opt_min_depth</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a34129c2b34975a16ff949a4f37ee7677</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::optional&lt; uint16_t &gt;</type>
      <name>m_opt_max_depth</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>af0f25abb4f027353c6fe950b58ee73b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>m_display_nb_frames_skipped</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a820e0a8fbd29dca0e1a7409968018e0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>m_frame_cnt</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a50adefa6f01edddb230f4150e26a48fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>unsigned int</type>
      <name>m_features_thickness</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a80f8584ccbe46c30de415dbce184b417</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>FeaturesType</type>
      <name>m_features_type</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a86314a6022493e53123271b820402437</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::optional&lt; VISP_NAMESPACE_ADDRESSING vpCameraParameters &gt;</type>
      <name>m_opt_rgb_cam</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a229769ce7c87e174037990aeccfa7ad3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_run</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a6d6cade64b8882f4ba2c0286f43eac61</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::mutex</type>
      <name>m_mutex_run</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a3cb736cb69978a8014437f71a906eda0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::string &gt;</type>
      <name>m_client_nodes_name</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>aa3a2c96302ac2d289dd537ea68a26ae3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration" protection="protected">
      <type></type>
      <name>FeaturesType</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a7c196dd620fac2496c38a1f84b6e8f98</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>POINT</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a7c196dd620fac2496c38a1f84b6e8f98aefbfa9f1075046370c42dd4de721496a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue" protection="protected">
      <name>CROSS</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a7c196dd620fac2496c38a1f84b6e8f98a1608db77b8b911a3aa3aefc77df1af49</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef" protection="protected">
      <type>enum visp_tracker_common::TrackerGUI::FeaturesType</type>
      <name>FeaturesType</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>ad0867af25c3381e1fdaa22127ec79ce2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static std::string</type>
      <name>featuresTypeToString</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>afd35c3bc37b70d66c9a980a1c3564357</anchor>
      <arglist>(const FeaturesType &amp;mode)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static FeaturesType</type>
      <name>featuresTypeFromString</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a117cfa1fd8dd7c0a2ee60218b20eb0c3</anchor>
      <arglist>(const std::string &amp;name)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static std::string</type>
      <name>getAvailableFeaturesType</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>ad9dc956b7e20066004b71813deef71b8</anchor>
      <arglist>(const std::string &amp;prefix=&quot;&lt; &quot;, const std::string &amp;sep=&quot; , &quot;, const std::string &amp;suffix=&quot; &gt;&quot;)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>camera_info_callback</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a61830266b6cb5a3ad562aa46fe7da752</anchor>
      <arglist>(const sensor_msgs::msg::CameraInfo::SharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>image_callback</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>af233bb3925d57e00a06dd95ef3a20b27</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>depth_callback</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a196ba4e2c609d93c0c11863a9f17560e</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>info_callback</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>aefd684f4a3021fe4a376a68baa3ba0fa</anchor>
      <arglist>(const visp_tracker_common::msg::InfoStrings::ConstSharedPtr msg)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>switch_tracking</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a0f0726865c764a021f8cd3ed7e9e067d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>switch_visualization</name>
      <anchorfile>classvisp__tracker__common_1_1TrackerGUI.html</anchorfile>
      <anchor>a677f3dc6d8c70c7968f6cbbd4bf9768d</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
</tagfile>

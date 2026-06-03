<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.8">
  <compound kind="struct">
    <name>visp_mbt::MBTTracker::ExtrinsicsHelper</name>
    <filename>structvisp__mbt_1_1MBTTracker_1_1ExtrinsicsHelper.html</filename>
    <member kind="variable">
      <type>std::string</type>
      <name>m_frame_name</name>
      <anchorfile>structvisp__mbt_1_1MBTTracker_1_1ExtrinsicsHelper.html</anchorfile>
      <anchor>a21288c3a68e3a95b64153700ba506ce7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::string</type>
      <name>m_tracker_name</name>
      <anchorfile>structvisp__mbt_1_1MBTTracker_1_1ExtrinsicsHelper.html</anchorfile>
      <anchor>a8a6ecaff7ca5dfb6b6e1ab07e2ae54e7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>visp_mbt::MBTTracker</name>
    <filename>classvisp__mbt_1_1MBTTracker.html</filename>
    <class kind="struct">visp_mbt::MBTTracker::ExtrinsicsHelper</class>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>init</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a83816df458f570939c42e7c71b50f29d</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>treat_rgb</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a30abb8d8bb27c832d2954693331c03e8</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;rgb) override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>treat_depth</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ae6f41e7105936394d4cf18dafbf1af06</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;depth) override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>init_tracker</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a5cdd9e279c0636132180b694e8636f2e</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>init_from_xml</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a8543e1ffdf27f38c65470fae874f7747</anchor>
      <arglist>(const std::string &amp;config_file_path)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>init_from_json</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a7fdd5a2d6f2d093f02763f0b07ceb733</anchor>
      <arglist>(const std::string &amp;config_file_path)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>init_info_strings</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a5d8375a5158f64ade30e2aa2b13f531c</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>check_requires_depth</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ae5fe804fd73b51c16a8265eed70ef8d4</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>track</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ab6f1f668e37682326825a192adad4e82</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>init_tracking</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ae8fef6434658e7e22549903983387655</anchor>
      <arglist>(vpHomogeneousMatrix &amp;cMo, bool &amp;display_frame)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>perform_tracking</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ac8ace2d002c0aeb9e9acf350c2c6e5f0</anchor>
      <arglist>(vpHomogeneousMatrix &amp;cMo, std::vector&lt; std::string &gt; &amp;vec_info)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static visp_tracker_common::msg::NamedFeature</type>
      <name>mbt_model_to_msg</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ae83cbca322bbc6b3ba8f5811f5e23455</anchor>
      <arglist>(const std::vector&lt; std::vector&lt; double &gt; &gt; &amp;model, const std::string &amp;name, const rclcpp::Logger &amp;logger)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static bool</type>
      <name>check_model_parameter</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a1667d95797371a21c316278a90757522</anchor>
      <arglist>(const rclcpp::Parameter &amp;param, const rclcpp::Logger &amp;logger, const bool &amp;required, std::string &amp;value)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static std::string</type>
      <name>trackerTypeToStr</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a941b94809ed8a07c96592b0ad3741d73</anchor>
      <arglist>(const vpMbGenericTracker::vpTrackerType &amp;type)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static vpMbGenericTracker::vpTrackerType</type>
      <name>trackerTypeFromStr</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a5990a91ba46347339730442d4bf4b2a3</anchor>
      <arglist>(const std::string &amp;name)</arglist>
    </member>
    <member kind="function" protection="protected" static="yes">
      <type>static std::string</type>
      <name>getAvailableTrackerType</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a3f43f3303192f6aa91791168f859ae12</anchor>
      <arglist>(const std::string &amp;prefix=&quot;&lt; &quot;, const std::string &amp;sep=&quot; , &quot;, const std::string &amp;suffix=&quot; &gt;&quot;)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_init_file_path</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a64525948772c88a3556afea9d61641b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_load_models_from_params</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a673aa3a68d822c1a6a2ff3a2cf4f09e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_rgb_model</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a91ed867f359dee726129b98b22998f2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_depth_model</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a56c2c08ab1f23e402bbf186d484ef559</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::string &gt;</type>
      <name>m_color_trackers_name</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a3e9a66387e30c5d4e70bea4cbfc2cb37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; std::string &gt;</type>
      <name>m_depth_trackers_name</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a38396a967b9324bfd2695ee798d72ace</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ExtrinsicsHelper</type>
      <name>m_ref_cam</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>afab27564a762d6f47189a9d26d69e3c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ExtrinsicsHelper</type>
      <name>m_other_cam</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a44944fc67aa91a23474a8bb9196c31c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_extrinsics_from_tf</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a67cf505f2a4a8b8abbe8d882a2a85115</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_extrinsics_set</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a530e1ef054c0a01d755576b5e94f7ca2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_tracker_initialized</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a1f32e9e9ada61eebe135baaecddbd72b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_tracker_cams_set</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>aea00d8ee88540d31ccf8502b1b056d09</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_must_detect_failure</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ab4c5f17c979aecbf4c3689549759f8bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>double</type>
      <name>m_projection_error_thresh</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a3969552d75edc42a435f1b1b70ed1fdb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>vpImage&lt; unsigned char &gt;</type>
      <name>m_I</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a06f384558b302c9af5f8670f0c26ea37</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>vpImage&lt; vpRGBa &gt;</type>
      <name>m_Ic</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>afa49711986d8924d07cb0fd2492ce495</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::vector&lt; vpColVector &gt;</type>
      <name>m_pointcloud</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ace6f1c1e4eaf4c1c6bca257b88b0570e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::map&lt; std::string, const vpImage&lt; unsigned char &gt; * &gt;</type>
      <name>m_map_img</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a45474321fbe5ed9d99719e2c70251bd3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::map&lt; std::string, const std::vector&lt; vpColVector &gt; * &gt;</type>
      <name>m_map_pc</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a3543390bb0f38bd1d077a515b9719003</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::map&lt; std::string, unsigned int &gt;</type>
      <name>m_map_pcw</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>aac6ce37f701c6737695311f8f0534ea4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::map&lt; std::string, unsigned int &gt;</type>
      <name>m_map_pch</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a8a069a2125b68b2ec363e39ef187a3b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>init_tracker</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a5cdd9e279c0636132180b694e8636f2e</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>init_from_xml</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a8543e1ffdf27f38c65470fae874f7747</anchor>
      <arglist>(const std::string &amp;config_file_path)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>init_from_json</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a7fdd5a2d6f2d093f02763f0b07ceb733</anchor>
      <arglist>(const std::string &amp;config_file_path)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>init_info_strings</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>a5d8375a5158f64ade30e2aa2b13f531c</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>check_requires_depth</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ae5fe804fd73b51c16a8265eed70ef8d4</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>track</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ab6f1f668e37682326825a192adad4e82</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>init_tracking</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ae8fef6434658e7e22549903983387655</anchor>
      <arglist>(vpHomogeneousMatrix &amp;cMo, bool &amp;display_frame)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>perform_tracking</name>
      <anchorfile>classvisp__mbt_1_1MBTTracker.html</anchorfile>
      <anchor>ac8ace2d002c0aeb9e9acf350c2c6e5f0</anchor>
      <arglist>(vpHomogeneousMatrix &amp;cMo, std::vector&lt; std::string &gt; &amp;vec_info)</arglist>
    </member>
  </compound>
</tagfile>

<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.8">
  <compound kind="class">
    <name>visp_rbt::RBTTracker</name>
    <filename>classvisp__rbt_1_1RBTTracker.html</filename>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>treat_rgb</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>af20405d3ac422bf3b8b59f811fa47b9c</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;rgb) override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>treat_depth</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a27ce6478d655a2ffaba9326f378a29dc</anchor>
      <arglist>(const sensor_msgs::msg::Image::ConstSharedPtr &amp;depth) override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>track</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>afd0bb3fe54b57ca075ff699ca26331bd</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>init_tracking</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a6b8726e1d0fc835aeff41cc911daedac</anchor>
      <arglist>(VISP_NAMESPACE_ADDRESSING vpHomogeneousMatrix &amp;cMo, bool &amp;display_frame)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>perform_tracking</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a297020573b1490bd3657210b17c3c87f</anchor>
      <arglist>(VISP_NAMESPACE_ADDRESSING vpHomogeneousMatrix &amp;cMo, std::vector&lt; std::string &gt; &amp;vec_info)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>init_tracker</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a335fc3bf13bcc480c1de68b25e3a67ac</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>init_from_json</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>ab886f318a270cdb805be85d2805e969d</anchor>
      <arglist>(const std::string &amp;config_file_path)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>init_info_strings</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a8ee1b287d5ca2b183904ec57d44e8964</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>check_requires_depth</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a305370e14f4c5e271911b9dcfb31edbd</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_tracker_initialized</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>ae930c37bd0122022943a9a65c7195cf4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_tracker_cams_set</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a40690ca7adde61a11dbd3562b2ccf40b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>m_init_file_path</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a939618c2feb8a76da73cd25a608aa8bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>VISP_NAMESPACE_ADDRESSING vpImage&lt; unsigned char &gt;</type>
      <name>m_I</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>aae3e038b9ac87e4b96b863f36e60cc6f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>VISP_NAMESPACE_ADDRESSING vpImage&lt; VISP_NAMESPACE_ADDRESSING vpRGBa &gt;</type>
      <name>m_Ic</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a992c592a9265d3a2b316a638547cd0a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>VISP_NAMESPACE_ADDRESSING vpImage&lt; float &gt;</type>
      <name>m_I_depth</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a75ed11e41ee7f8f7a498dcede4a31628</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>init_tracker</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a335fc3bf13bcc480c1de68b25e3a67ac</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>bool</type>
      <name>init_from_json</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>ab886f318a270cdb805be85d2805e969d</anchor>
      <arglist>(const std::string &amp;config_file_path)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>init_info_strings</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a8ee1b287d5ca2b183904ec57d44e8964</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual void</type>
      <name>check_requires_depth</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a305370e14f4c5e271911b9dcfb31edbd</anchor>
      <arglist>() override</arglist>
    </member>
  </compound>
</tagfile>

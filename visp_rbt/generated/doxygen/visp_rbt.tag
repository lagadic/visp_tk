<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.8">
  <compound kind="class">
    <name>visp_rbt::RBTTracker</name>
    <filename>classvisp__rbt_1_1RBTTracker.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>init</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a1f5c511a5afda83200a8604d9f23c1d8</anchor>
      <arglist>() override</arglist>
    </member>
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
      <anchor>a6d8e5b66a302140c3b0128dbfe2888ba</anchor>
      <arglist>(visp::vpHomogeneousMatrix &amp;cMo, bool &amp;display_frame)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>perform_tracking</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a1611d086c9fa02f5ce0c16ff3cffdb59</anchor>
      <arglist>(visp::vpHomogeneousMatrix &amp;cMo, std::vector&lt; std::string &gt; &amp;vec_info)</arglist>
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
      <name>m_tracker_cams_set</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a40690ca7adde61a11dbd3562b2ccf40b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>bool</type>
      <name>m_load_model_from_params</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a2c76651b05153191363597a6c2dac98d</anchor>
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
      <type>visp::vpImage&lt; unsigned char &gt;</type>
      <name>m_I</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>ab4097e9ba2d03cfa7c6d9ddf39a462ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>visp::vpImage&lt; visp::vpRGBa &gt;</type>
      <name>m_Ic</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a3537d10c44b37cc5cfd34568cf596ae3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>visp::vpImage&lt; float &gt;</type>
      <name>m_I_depth</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a12c61e21c794d391f1ba1513c052bd4f</anchor>
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

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
      <anchor>a4aa7558637f7a328c52543a6d4a71fec</anchor>
      <arglist>(vpHomogeneousMatrix &amp;cMo, bool &amp;display_frame)</arglist>
    </member>
    <member kind="function" protection="protected" virtualness="virtual">
      <type>virtual bool</type>
      <name>perform_tracking</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a90ac5ade12d73d59c3449c48463ba77e</anchor>
      <arglist>(vpHomogeneousMatrix &amp;cMo, std::vector&lt; std::string &gt; &amp;vec_info)</arglist>
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
      <type>vpImage&lt; unsigned char &gt;</type>
      <name>m_I</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a5031ff1e667ebdcc766840e6836c0e5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>vpImage&lt; vpRGBa &gt;</type>
      <name>m_Ic</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a3b9d5e8036421b39ba9e591161f12f54</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>vpImage&lt; float &gt;</type>
      <name>m_I_depth</name>
      <anchorfile>classvisp__rbt_1_1RBTTracker.html</anchorfile>
      <anchor>a268367adcf98ce01e6f912fc6420664a</anchor>
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

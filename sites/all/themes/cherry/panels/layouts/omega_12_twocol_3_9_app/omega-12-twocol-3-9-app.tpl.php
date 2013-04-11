<div class="panel-display omega-grid omega-12-twocol-3-9" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="panel-panel grid-3 left-above">
    <div class="inside"><?php print $content['left_above']; ?></div>
  </div>  
  <div class="panel-panel grid-9 right-above">
	<div class="inside"><?php print $content['right_above']; ?></div>
  </div>
  <div class="panel-panel grid-12 middle"></div>
  <div class="panel-panel grid-3 left-below">
	<div class="inside"><?php print $content['left_below']; ?></div>
  </div>
  <div class="panel-panel grid-9">
	<div id="tabs" class="inside">
      <ul>
        <li><a href="#tabs-1">Overview</a></li>
        <li><a href="#tabs-2">User reviews</a></li>
        <li><a href="#tabs-3">What's new</a></li>
        <li><a href="#tabs-4">Permissions</a></li>
      </ul>
      <div id="tabs-1"><?php print $content['tab_1']; ?></div>
      <div id="tabs-2"><?php print $content['tab_2']; ?></div>
      <div id="tabs-3"><?php print $content['tab_3']; ?></div>
      <div id="tabs-4"><?php print $content['tab_4']; ?></div>
    </div>
  </div>
</div>

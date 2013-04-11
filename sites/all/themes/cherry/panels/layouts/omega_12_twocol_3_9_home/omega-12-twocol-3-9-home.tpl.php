<div class="panel-display omega-grid omega-12-twocol-3-9" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="panel-panel grid-12 top">
    <div class="inside"><?php print $content['top']; ?></div>
  </div>  
  <div class="panel-panel grid-3 left">
    <div class="inside"><?php print $content['left']; ?></div>
  </div>  
  <div class="panel-panel grid-9 right">
	<div class="inside"><?php print $content['right']; ?></div>
  </div>
</div>

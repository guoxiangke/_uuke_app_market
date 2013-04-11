<?php
/**
 * @file
 * Template for a 2 column panel layout.
 *
 * This template provides a two column panel display layout, with
 * additional areas for the top and the bottom.
 *
 * Variables:
 * - $id: An optional CSS id to use for the layout.
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout. This layout supports the following sections:
 *   - $content['top']: Content in the top row.
 *   - $content['secondary']: Content in the secondary column.
 *   - $content['right']: Content in the right column.
 *   - $content['bottom']: Content in the bottom row.
 */
?>
<?php if ($content['header']): ?>
  <div data-role="header" data-theme="b" data-position="fixed" style="text-align: center">
    <?php print $content['header']; ?>
  </div>
<?php endif; ?>
<div data-role="content">
<?php if ($content['secondary']): ?>
  <div class="content-secondary">
    <?php print $content['secondary']; ?>
  </div>
  <div class="content-primary">
    <?php print $content['primary']; ?>
  </div>
<?php else: ?>
    <?php print $content['primary']; ?>
<?php endif; ?>
</div>

<?php

/**
 * @file
 * Definition of the 'download' panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Download style'),
  'description' => t('The download panel rendering style; displays each file with a link.'),
  'render pane' => 'panels_download_style_render_pane',
);

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function cherry_panels_download_style_render_pane($vars) {
  $output = '';
  $output .= '<a class="download" href="' . $vars['content']->content[0]['#markup'] . '">安装</a>';
  return $output;
}


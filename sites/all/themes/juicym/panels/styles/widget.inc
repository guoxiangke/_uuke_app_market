<?php
/**
* Implementation of hook_panels_styles().
*/

// Plugin definition
$plugin = array(
  'title' => t('Mobi widget'),
  'description'	=> t('Mobi widget panel style plugin'),
  'render pane'  => 'widget_style_pane',
  'hook theme' => array(
    'widget' => array(
      'variables' => array('content' => NULL),
      'path' => drupal_get_path('theme', 'juicym') .'/panels/styles',
      'template' => 'widget',
    ),
  ),
);

function theme_widget_style_pane($vars){
  $content = $vars['content'];
  $output = theme('widget', array('title' => t($content->title), 
  	'content' => $content->content));
  return $output;
}

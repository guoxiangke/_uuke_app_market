<?php

/**
 * @file
 * Definition of the 'download' panel style.
 */

// Plugin definition
$plugin = array(
	'title' => t('Download button'),
  	'description' => t('The download rendering style; displays each file with a button.'),
	'render pane'  => 'download_style_pane',
	'hook theme' => array(
		'download' => array(
				'variables' => array('content' => NULL),
				'path' => drupal_get_path('theme', 'juicym') .'/panels/styles',
				'template' => 'download',
		),
	),
);

function theme_download_style_pane($vars){
	$uri = file_create_url($vars['content']->content[0]['#file']->uri);
	$output = theme('download', array('uri' => $uri));
	return $output;
}
		
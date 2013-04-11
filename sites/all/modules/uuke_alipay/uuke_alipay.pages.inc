<?php
/**
 * 
 * 
 */
function uuke_alipay_content_types_setting_form($value=''){
	$form = array();
	$form['titles'] = array(
		'#markup' => t('可以购买的节点的类型,自动添加字段field_uuke_price to the content type')
	);
	$synch_types = variable_get('uuke_alipay_content_types', NULL);
  if(!$synch_types){
    $synch_types = array();
  }else{
    $synch_types = array_filter(unserialize($synch_types));
  }
  $node_types = node_type_get_types();
  foreach ($node_types as $node_type => $node_type_obj) {
    $form['node_types']["$node_type"] = array(
      '#title'=>$node_type_obj->name,
      '#type' => 'checkbox',
      '#default_value' =>key_exists($node_type_obj->type, $synch_types)?TRUE:FALSE,
      '#description' => $node_type_obj->description,
    );
  }
	// Add the buttons.
	$form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );
	$form['actions']['reset'] = array(
    '#type' => 'button',
    '#value' => t('reset'),
    '#weight' => 6,
  );
	return $form;
}

/**
 * Form submit handler for the open_users_synch_setting() form.
 */
function uuke_alipay_content_types_setting_form_submit($form, &$form_state){
	$node_types = node_type_get_types();
	$setting = array();
	foreach ($node_types as $node_type => $node_type_obj){
		$setting[$node_type] = $form_state['values'][$node_type];
	}
	$synch_types = serialize($setting);
	if(variable_get('uuke_alipay_content_types', null)==$synch_types){
  	drupal_set_message(t('设置未更改！'),'status');
	}else{
    dpm($setting,'$setting');
	  variable_set('uuke_alipay_content_types', $synch_types);
    foreach ($setting as $content_type => $value) {
      $data = array(
        'instances' => array(
          'field_uuke_price' => array(
            0 => array(
              'label' => 'Price',
              'widget' => array(
                'type' => 'number',
                'weight' => '5',
                'settings' => array(),
                'module' => 'number',
              ),
              'settings' => array(
                'min' => '',
                'max' => '',
                'prefix' => '',
                'suffix' => '',
                'user_register_form' => FALSE,
              ),
              'display' => array(
                'default' => array(
                  'label' => 'above',
                  'type' => 'number_decimal',
                  'settings' => array(
                    'thousand_separator' => ' ',
                    'decimal_separator' => '.',
                    'scale' => 2,
                    'prefix_suffix' => TRUE,
                  ),
                  'module' => 'number',
                  'weight' => 9,
                ),
                'teaser' => array(
                  'type' => 'hidden',
                  'label' => 'above',
                  'settings' => array(),
                  'weight' => 0,
                ),
              ),
              'required' => FALSE,
              'description' => '',
              'field_name' => 'field_uuke_price',
              'entity_type' => 'node',
              'bundle' => $content_type,
              'deleted' => '0',
              'default_value' => NULL,
            ),
          ),
        ),
      );
      if($value){//add instances
        foreach ($data['instances'] as $instances_name => $instances) {
          foreach ($instances as $key => $instance) {
            //$instance = field_info_instance('node', 'body', $type->type);
            $ori_instance = field_info_instance('node', $instances_name, $instance['bundle']);
            if (empty($ori_instance)) {
               $instance = field_create_instance($instance);
               watchdog('uuke_alipay', 'created instance for '.$content_type.': '.$instances_name);
            }
          }
        }
      }else{//del instances

        // foreach ($data['instances'] as $instances_name => $instances) {
        //   foreach ($instances as $key => $instance) {
        //     //$instance = field_info_instance('node', 'body', $type->type);
        //     $ori_instance = field_info_instance('node', $instances_name, $instance['bundle']);
        //     if (empty($ori_instance)) {
        //        $instance = field_delete_instance($instance,FALSE);
        //        watchdog('uuke_alipay', 'delete instance for '.$content_type.': '.$instances_name);
        //     }
        //   }
        // }
      }
    }
		drupal_set_message(t('设置成功！'),'status');
    // drupal_set_message(t('请手动删除取消的content_typ field_uuke_price字段'),'status');
	}
}


function uuke_alipay_paid_list($account=NUll,$admin=FALSE) {
  global $user;
  if(!$account)  $account = $user;
  //buy online       
  $header = array(t('支付宝交易号'), t('商家订单号'),t('卖家商品名称'), t('价格'), t('数量'), t('下单时间'), t('付款时间'));
  // $header = array(t('Name'), t('Weight'), array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();
  $query = db_select('uuke_alipay', 'c')
  ->fields('c')
  ->orderBy('paid_time', 'DESC');
  if(!$admin){
    $query = $query
      ->condition('uid', $account->uid);
  }else{
    drupal_set_title('All paid list');
  }
  $result = $query->execute();
  while(
    $record = $result->fetchAssoc()) {
    $row = array();
    $product_node  = node_load($record['nid']);
    $title = truncate_utf8($product_node->title, 20, $wordsafe = FALSE, $add_ellipsis = 1, $min_wordsafe_length = 1);
    $row[] = $record['trade_no'];//支付宝交易号
    $row[] = $record['out_trade_no'];//商家订单号    
    $row[] = l($title,'node/'.$record['nid']);//卖家商品名称   
    // $row[] = $record['uid'];// 
    $row[] = $record['price'];//价格    
    $row[] = $record['quantity'];//数量
    $row[] = date('Y-m-d h:i:m',$record['buy_time']);//xia单时间    
    $row[] = date('Y-m-d h:i:m',$record['paid_time']);//付款时间
    $rows[] = array('data' => $row, 'class' => array(''));
  }
// CREATE TABLE IF NOT EXISTS `uuke_alipay` (
//   `trade_no` varchar(255) NOT NULL DEFAULT '' COMMENT '支付宝交易号r',
//   `out_trade_no` varchar(255) NOT NULL DEFAULT '' COMMENT '商家交易号',
//   `uid` int(11) NOT NULL DEFAULT '0' COMMENT 'User’s users.uid.',
//   `nid` int(10) unsigned DEFAULT NULL COMMENT '商品节点id',
//   `price` varchar(255) NOT NULL DEFAULT '',
//   `quantity` int(11) NOT NULL DEFAULT '0',
//   `paid_time` int(11) NOT NULL DEFAULT '0' COMMENT '付款时间',
//   `buy_time` int(11) NOT NULL DEFAULT '0' COMMENT '订单生产时间',
//   PRIMARY KEY (`trade_no`),
//   KEY `pid` (`uid`),
//   KEY `nid` (`nid`)
// ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Stores register pads.';

  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'uuke_alipay_paid_list')));
  return $output;
}
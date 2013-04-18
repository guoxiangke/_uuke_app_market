<?php
// 'version'       => '1.0.0',
// 'charset'       => 'UTF-8', //UTF-8, GBK等
// 'merId'         => '105550149170027', //商户填写
// 'acqCode'       => '',  //收单机构填写
// 'merCode'       => '',  //收单机构填写
// 'merAbbr'       => '商户名称',
function uuke_unionpay_configue_form($form, &$form_state) {
  $form['uuke_unionpay_merId'] = array(
    '#type' => 'textfield',
    '#title' => t('商户代码'),
    '#description' => t('merId.'),
    '#required' => TRUE,
    '#default_value' => variable_get('uuke_unionpay_merId', '105550149170027'),
    '#size' => 16,
  );  
  $form['uuke_unionpay_security_key'] = array(
    '#type' => 'textfield',
    '#title' => t('商户密钥'),
    '#required' => TRUE,
    '#default_value' => variable_get('uuke_unionpay_security_key', '88888888'),
    '#description' => 'Q：银联Upop开发测试环境支付的测试卡号、密码？
A：借记卡卡号：6212341111111111111，密码：111111，手机号：13888888888，短信验证码测试环境不校验，可随便输6位数字。
    信用卡卡号：6212341111111111111，年份：12，月份：12，CVN2：123，短信验证码',
    '#size' => 16,
  );
  $form['uuke_unionpay_acqCode'] = array(
    '#type' => 'textfield',
    '#title' => t('uuke_unionpay_acqCode.'),
    '#description' => t('商户参数.收单机构填写'),
    '#default_value' => variable_get('uuke_unionpay_acqCode', ''),
  );
  $form['uuke_unionpay_merCode'] = array(
    '#type' => 'textfield',
    '#title' => t('uuke_unionpay_merCode.'),
    '#description' => t('商户参数.收单机构填写'),
    '#default_value' => variable_get('uuke_unionpay_merCode', ''),
  );
  $form['uuke_unionpay_merAbbr'] = array(
    '#type' => 'textfield',
    '#title' => t('商户名称.'),
    '#default_value' => variable_get('uuke_unionpay_merAbbr', ''),
  );
	//交易类型，CONSUME or PRE_AUTH
  $form['uuke_unionpay_transType'] = array(
    '#title' => t('交易种类'),
    '#type' => 'radios',
    '#default_value' => variable_get('uuke_unionpay_transType', 'CONSUME'),
    '#options' => array(
      'CONSUME' => t('直接消费'),
      'PRE_AUTH' => t('担保消费'),
    )
  );
  $form['uuke_unionpay_environment'] = array(
    '#title' => t('运行模式'),
    '#type' => 'radios',
    '#default_value' => variable_get('uuke_unionpay_environment', '1'),
    '#options' => array(
      '1' => t('测试环境'),
      '2' => t('预上线环境'),
      '3' => t('线上环境'),
    )
  );
  return system_settings_form($form);
}
function uuke_unionpay_notify(){
	try {
	    $response = new quickpay_service($_POST, quickpay_conf::RESPONSE);
	    if ($response->get('respCode') != quickpay_service::RESP_SUCCESS) {
	        $err = sprintf("Error: %d => %s", $response->get('respCode'), $response->get('respMsg'));
	        throw new Exception($err);
	    }

	    $arr_ret = $response->get_args();

	    //更新数据库，将交易状态设置为已付款
	    //注意保存qid，以便调用后台接口进行退货/消费撤销
	    watchdog('uuke_unionpay_notify:更新数据库，将交易状态设置为已付款', '<pre>'.print_r($arr_ret,TRUE), array(), WATCHDOG_NOTICE, 'link');
	    //以下仅用于测试
	    // file_put_contents('notify.txt', var_export($arr_ret, true));

	}
	catch(Exception $exp) {
	    //后台通知出错
	    watchdog('uuke_unionpay_notify:后台通知出错', '<pre>'.print_r($exp,TRUE), array(), WATCHDOG_NOTICE, 'link');
	    // file_put_contents('notify.txt', var_export($exp, true));
	}
	echo 'sucess';
}
function uuke_unionpay_return() {
	try {
	    $response = new quickpay_service($_POST, quickpay_conf::RESPONSE);
	    if ($response->get('respCode') != quickpay_service::RESP_SUCCESS) {
	        $err = sprintf("Error: %d => %s", $response->get('respCode'), $response->get('respMsg'));
	        throw new Exception($err);
	    }
	    $arr_ret = $response->get_args(); 
	    dpm($_POST,'$_POST');
	    dpm($arr_ret,'$arr_ret');
	    //告诉用户交易完成
	    return "订单 {$arr_ret['orderNumber']} 支付成功";

	}
	catch(Exception $exp) {
	    $str .= var_export($exp, true);
	    die("error happend: " . $str);
	}



  //获取支付宝的反馈参数
  $out_trade_no   = $_REQUEST['out_trade_no'];   //获取支付宝传递过来的订单号
  $order_node = node_load($out_trade_no-0);
  $product_node = node_load($order_node->field_uuke_alipay_product_nid[LANGUAGE_NONE][0]['target_id']);
  $trade_no =  $_REQUEST['trade_no'];
  $trade_status = $_REQUEST['trade_status'];
  // $body = $_REQUEST['body'];

  $partner = variable_get('uuke_alipay_partner', '');
  $security_code = variable_get('uuke_alipay_key', '');
  $sign_type = "MD5";
  $_input_charset = 'UTF-8';
  $transport = 'https';
  $alipay = new alipay_notify($partner,$security_code,$sign_type,$_input_charset,$transport);
  $verify_result = $alipay->return_verify();
  dpm($_REQUEST,'$_REQUEST');
  if($verify_result) {    //认证合格
    if($trade_status == 'WAIT_SELLER_SEND_GOODS') {
      // create a now order node begin
      $request = $_REQUEST;

      $order = uuke_alipay_order_save($request);
  //执行自动发货 begin
  uuke_alipay_auto_send_goods($trade_no,$out_trade_no);
  //执行自动发货 end
      // create a now order node end
      drupal_set_message(t('付款成功!.'), 'status', FALSE);
      drupal_goto('node/'.$product_node->nid);
  }elseif($trade_status == 'TRADE_FINISHED' || $trade_status == 'TRADE_SUCCESS') {
    //这里放入你自定义代码,比如根据不同的trade_status进行不同操作
    //log_result("verify_success"); 
      drupal_set_message(t('已购买，请点此下载安装App.'), 'status', FALSE);
      drupal_goto('node/'.$product_node->nid);
    }else{
      drupal_set_message(t('unknow error: Please conneect admin. trade_status:'.$_REQUEST['trade_status']), 'warning', FALSE);
      drupal_goto('node/'.$product_node->nid);
    }
  }
  else {//认证不合格
    $output = t('您的交易信息是无效的，如果你使用的是已有的返回信息的话，那么交易已经成立，你可以查看订单历史。如果你是黑客的话，俺是守法的好公民，希望你能饶了我这个小店');
    //echo "fail";
    //log_result ("verify_failed");
  }
  return '';
}
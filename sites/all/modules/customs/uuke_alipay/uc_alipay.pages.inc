	<?php

	/**
	 * @file
	 *集成支付宝的支付服务.
	 *zhupou.cn制作
	 */

	function uc_alipay_notify() {
		//$output = "uc_alipay_return test";
		$partner = variable_get('uuke_alipay_partner', '');
		$security_code = variable_get('uuke_alipay_key', '');
		$sign_type = "MD5";
		$_input_charset = 'UTF-8';
		$transport = 'https';
		watchdog('uc_alipay_notify_called at', REQUEST_TIME);
		if(!isset($_POST['out_trade_no'])) return 'error,this page only called by alipay nofify!';
		$alipay = new alipay_notify($partner,$security_code,$sign_type,$_input_charset,$transport);
		$verify_result = $alipay->notify_verify();
		if($verify_result) {  //认证合格
		 //获取支付宝的反馈参数
		  $out_trade_no   = $_POST['out_trade_no'];   //获取支付宝传递过来的订单号
			$order_node = node_load($out_trade_no-0);
			$product_node = node_load($order_node->field_uuke_alipay_product_nid[LANGUAGE_NONE][0]['target_id']);
			$nid = $order_node->nid;
			$uid = $order_node->uid;
			$buy_timestamp = $order_node->created;

			// preg_match('/n\d+/', $out_trade_no, $matches, PREG_OFFSET_CAPTURE);
		 //  $nid = str_replace('n', '',$matches[0][0]);//order_id=200 将前面的11位删除 1365500773_n200_u1  n\d+
		 //  $product_node = node_load($nid);//product_type
			// preg_match('/u\d+/', $out_trade_no, $matches, PREG_OFFSET_CAPTURE);
		 //  $uid = str_replace('u', '',$matches[0][0]);//200 将前面的11位删除 1365500773_n200_u1  n\d+
			// preg_match('/\d+/', $out_trade_no, $matches, PREG_OFFSET_CAPTURE);
			// $buy_timestamp = $matches[0][0];//下单时间
			
			$trade_no =  $_POST['trade_no'];
		  $total_fee     = $_POST['total_fee'];      //获取支付宝传递过来的总价格
		  $price =  $_POST['price'];
			$quantity =  $_POST['quantity'];
			$trade_status = $_POST['trade_status'];//WAIT_BUYER_PAY
			$trade_status_array = array(
				'WAIT_BUYER_PAY',
				'TRADE_FINISHED',
				'TRADE_SUCCESS',
				'WAIT_SELLER_SEND_GOODS',
				'TRADE_CLOSED',
				);
			if(!in_array($trade_status,$trade_status_array)) {
					watchdog('uc_alipay_notify', $trade_status.':验证失败1,状态未知 out_trade_no='.$out_trade_no.'&trade_no='.$trade_no);
					echo "fail";
					//log_result ("verify_failed");
			}
			// $trade_status_in_db = uuke_alipay_trade_status_in_db($trade_status);//0 1 2 3 -1
			// $receive_name    =$_POST['receive_name'];    //获取收货人姓名
			// $receive_address =$_POST['receive_address']; //获取收货人地址
			// $receive_zip     =$_POST['receive_zip'];     //获取收货人邮编
			// $receive_phone   =$_POST['receive_phone'];   //获取收货人电话
			// $receive_mobile  =$_POST['receive_mobile'];  //获取收货人手机
			/* 
			   获取支付宝反馈过来的状态,根据不同的状态来更新数据库 
			   WAIT_BUYER_PAY(表示等待买家付款);
			   TRADE_FINISHED(表示交易已经成功结束);
		    */
			//执行自动发货 begin
	   	if($trade_status == 'WAIT_SELLER_SEND_GOODS'){
				$send_goods_confirm_link = send_goods_confirm_link($trade_no,$out_trade_no);
				//https://www.alipay.com/cooperate/gateway.do?_input_charset=UTF-8&invoice_no=1365556470_n1_u1&logistics_name=uuke_EXPRESS&partner=2088901533144817&service=send_goods_confirm_by_platform&trade_no=2013041040251316&transport_type=EXPRESS&sign=1700a5b303e8b6954df982e7ccaff52a&sign_type=MD5
				$response_xml = uuke_getHttpResponseGET($send_goods_confirm_link);
			}
			//执行自动发货 end
			//更新订单
			$request = $_POST;
			$order = uuke_alipay_order_save($request);
			// $query = db_select('uuke_alipay', 'h');
			// $result = $query
			//   ->fields('h', array('uid','nid','trade_no','status'))
			//   ->condition('h.trade_no ',$trade_no)
			//   ->execute()
			//   ->fetchAssoc();
	    //这里放入你自定义代码,比如根据不同的trade_status进行不同操作
			// if($result['status'] != $trade_status_in_db) {
			// 	// 交易状态改变
			// 	// $query = db_update('uuke_alipay')
			// 	// 	->fields(array(
			// 	// 		  'status' => $trade_status_in_db,
			// 	// 		))
		 //  // 		->condition('h.trade_no ',$trade_no)
			// 	// 	->execute();
			// 	watchdog('uc_alipay_notify', $trade_status.':已经记录,交易状态改变,out_trade_no='.$out_trade_no.'&trade_no='.$trade_no);
			// }else{
			// 	watchdog('uc_alipay_notify', $trade_status.':已经记录,交易wei改变,out_trade_no='.$out_trade_no.'&trade_no='.$trade_no);
			// }
			// else{
			// 	// db_insert('uuke_alipay', array())
	  // 	// 		->fields(array(
			// 	//   'trade_no' => $trade_no,
			// 	//   'out_trade_no' => $out_trade_no,
			// 	//   'uid' => $uid,
			// 	//   'nid' => $nid,
			// 	//   'paid_time' => REQUEST_TIME,
			// 	//   'buy_time' => $buy_timestamp,
			// 	//   'price' => $price,
			// 	//   'quantity' => $quantity,					  
			// 	//   'type' => $product_node->type,
			// 	//   'status' => $trade_status_in_db,
			// 	// 	))
			// 	// ->execute();
			// 	// create a now order node begin
			// 	$request = $_POST;
			// 	$order = uuke_alipay_order_save($request);
			// 	// create a now order node end
			// 	// drupal_set_message(t('付款成功!.'), 'status', FALSE);
			// 	watchdog('uc_alipay_notify', $trade_status.':交易 added to db!,out_trade_no='.$out_trade_no.'&trade_no='.$trade_no);
			// }
			echo 'success';
		}else {    //认证不合格
			watchdog('uc_alipay_notify', $trade_status.':认证不合格,out_trade_no='.$out_trade_no.'&trade_no='.$trade_no);
			echo "fail";
			//log_result ("verify_failed");
		}
	}

function uc_alipay_return() {
	//获取支付宝的反馈参数
	$out_trade_no   = $_GET['out_trade_no'];   //获取支付宝传递过来的订单号
	$order_node = node_load($out_trade_no-0);
	$product_node = node_load($order_node->field_uuke_alipay_product_nid[LANGUAGE_NONE][0]['target_id']);
	$trade_no =  $_GET['trade_no'];
	$trade_status = $_GET['trade_status'];
	// $body = $_GET['body'];
	// $trade_status_in_db = uuke_alipay_trade_status_in_db($trade_status);//0 1 2 3 -1
	//执行自动发货 begin
	$send_goods_confirm_link = send_goods_confirm_link($trade_no,$out_trade_no);
	//https://www.alipay.com/cooperate/gateway.do?_input_charset=UTF-8&invoice_no=1365556470_n1_u1&logistics_name=uuke_EXPRESS&partner=2088901533144817&service=send_goods_confirm_by_platform&trade_no=2013041040251316&transport_type=EXPRESS&sign=1700a5b303e8b6954df982e7ccaff52a&sign_type=MD5
	$response_xml = uuke_getHttpResponseGET($send_goods_confirm_link);
	//执行自动发货 end
	//require_once("uc_alipay_service.inc");
	$partner = variable_get('uuke_alipay_partner', '');
	$security_code = variable_get('uuke_alipay_key', '');
	$sign_type = "MD5";
	$_input_charset = 'UTF-8';
	$transport = 'https';
	$alipay = new alipay_notify($partner,$security_code,$sign_type,$_input_charset,$transport);
	$verify_result = $alipay->return_verify();
	dpm($_GET,'$_GET');
	if($verify_result) {    //认证合格
		if($trade_status == 'WAIT_SELLER_SEND_GOODS') {
			// create a now order node begin
			$request = $_GET;
			$order = uuke_alipay_order_save($request);
			// create a now order node end
			drupal_set_message(t('付款成功!.'), 'status', FALSE);
			drupal_goto('node/'.$product_node->nid);
	}elseif($trade_status == 'TRADE_FINISHED' || $trade_status == 'TRADE_SUCCESS') {
		//这里放入你自定义代码,比如根据不同的trade_status进行不同操作
		//log_result("verify_success"); 
			drupal_set_message(t('已购买，请点此下载安装App.'), 'status', FALSE);
			drupal_goto('node/'.$product_node->nid);
		}else{
			drupal_set_message(t('unknow error: Please conneect admin. trade_status:'.$_GET['trade_status']), 'warning', FALSE);
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

	/*
		*功能：付款过程中服务器通知类
		*版本：2.0
		*日期：2008-08-01
		*作者：支付宝公司销售部技术支持团队
		*联系：0571-26888888
		*版权：支付宝公司
		*/

	class alipay_notify {

		var $gateway;           //支付接口
		var $security_code;  	//安全校验码
		var $partner;           //合作伙伴ID
		var $sign_type;         //加密方式 系统默认
		var $mysign;            //签名     
		var $_input_charset;    //字符编码格式
		var $transport;         //访问模式
			
		function alipay_notify($partner,$security_code,$sign_type = "MD5",$_input_charset = "UTF-8",$transport= "https") {
			$this->partner        = $partner;
			$this->security_code  = $security_code;
			$this->sign_type      = $sign_type;
			$this->mysign         = "";
			$this->_input_charset = $_input_charset ;
			$this->transport      = $transport;
			if($this->transport == "https") {
				$this->gateway = "https://www.alipay.com/cooperate/gateway.do?";
			}else $this->gateway = "http://notify.alipay.com/trade/notify_query.do?";
		}
		
		
	/****************************************对notify_url的认证*********************************/
		
		function notify_verify() {   
			if($this->transport == "https") {
				$veryfy_url = $this->gateway. "service=notify_verify" ."&partner=" .$this->partner. "&notify_id=".$_POST["notify_id"];
			} else {
				$veryfy_url = $this->gateway. "partner=".$this->partner."&notify_id=".$_POST["notify_id"];
			}
			$veryfy_result = $this->get_verify($veryfy_url);
			$post          = $this->para_filter($_POST);
			$sort_post     = $this->arg_sort($post);
			while (list ($key, $val) = each ($sort_post)) {
				$arg.=$key."=".$val."&";
			}
			$prestr = substr($arg,0,count($arg)-2);  //去掉最后一个&号
			$this->mysign = $this->sign($prestr.$this->security_code);
			//log_result("notify_url_log:sign=".$_POST["sign"]."&mysign=".$this->mysign."&".$this->charset_decode(implode(",",$_POST),$this->_input_charset ));
			if (eregi("true$",$veryfy_result) && $this->mysign == $_POST["sign"])  {
				return true;
			} else return false;
		}
		
		
	/*******************************************************************************************/

	/**********************************对return_url的认证***************************************/

		function return_verify() {
			$arg ='';  
			$sort_get= $this->arg_sort($_GET);
			while (list ($key, $val) = each ($sort_get)) {
				if($key != "sign" && $key != "sign_type" && $key != "q")
				//$arg.=$key."=".$val."&";
				$arg.=$key."=".$this->charset_encode($val,'UTF-8','UTF-8')."&";
			}
			$prestr = substr($arg,0,count($arg)-2);  //去掉最后一个&号
			$this->mysign = $this->sign($prestr.$this->security_code);
			/*while (list ($key, $val) = each ($_GET)) {
			$arg_get.=$key."=".$val."&";
			}*/
			//log_result("return_url_log=".$_GET["sign"]."&".$this->mysign."&".$this->charset_decode(implode(",",$_GET),$this->_input_charset ));
			if ($this->mysign == $_GET["sign"])  return true;
			else return false;
		}
		
		
	/*******************************************************************************************/

		
		function get_verify($url,$time_out = "60") {
			$urlarr     = parse_url($url);
			$errno      = "";
			$errstr     = "";
			$transports = "";
			if($urlarr["scheme"] == "https") {
				$transports = "ssl://";
				$urlarr["port"] = "443";
			} else {
				$transports = "tcp://";
				$urlarr["port"] = "80";
			}
			$fp=@fsockopen($transports . $urlarr['host'],$urlarr['port'],$errno,$errstr,$time_out);
			if(!$fp) {
				die("ERROR: $errno - $errstr<br />\n");
			} else {
				fputs($fp, "POST ".$urlarr["path"]." HTTP/1.1\r\n");
				fputs($fp, "Host: ".$urlarr["host"]."\r\n");
				fputs($fp, "Content-type: application/x-www-form-urlencoded\r\n");
				fputs($fp, "Content-length: ".strlen($urlarr["query"])."\r\n");
				fputs($fp, "Connection: close\r\n\r\n");
				fputs($fp, $urlarr["query"] . "\r\n\r\n");
				while(!feof($fp)) {
					$info[]=@fgets($fp, 1024);
				}
				fclose($fp);
				$info = implode(",",$info);
				while (list ($key, $val) = each ($_POST)) {
					$arg.=$key."=".$val."&";
				}
			//	log_result("notify_url_log=".$url.$this->charset_decode($info,$this->_input_charset));
				//log_result("notify_url_log=".$this->charset_decode($arg,$this->_input_charset));
				return $info;
			}
		}
		
		
		function arg_sort($array) {
			ksort($array);
			reset($array);
			return $array;
		}

		
		function sign($prestr) {
			$sign='';
			if($this->sign_type == 'MD5') {
				$sign = md5($prestr);
			}elseif($this->sign_type =='DSA') {
				//DSA 签名方法待后续开发
				die("DSA 签名方法待后续开发，请先使用MD5签名方式");
			}else {
				die("支付宝暂不支持".$this->sign_type."类型的签名方式");
			}
			return $sign;
		}
		
	/***********************除去数组中的空值和签名模式*****************************/
		
		function para_filter($parameter) { 
			$para = array();
			while (list ($key, $val) = each ($parameter)) {
				if($key == "sign" || $key == "sign_type" || $val == "")continue;
				else	$para[$key] = $parameter[$key];
			}
			return $para;
		}
	/********************************************************************************/

	/******************************实现多种字符编码方式*****************************/
		
		function charset_encode($input,$_output_charset ,$_input_charset ="UTF-8" ) {
			$output = "";
			if(!isset($_output_charset) )$_output_charset  = $this->parameter['_input_charset '];
			if($_input_charset == $_output_charset || $input ==null ) {
				$output = $input;
			} elseif (function_exists("mb_convert_encoding")){
				$output = mb_convert_encoding($input,$_output_charset,$_input_charset);
			} elseif(function_exists("iconv")) {
				$output = iconv($_input_charset,$_output_charset,$input);
			} else die("sorry, you have no libs support for charset change.");
			return $output;
		}
	/********************************************************************************/

	/******************************实现多种字符解码方式******************************/
		
		function charset_decode($input,$_input_charset ,$_output_charset="UTF-8"  ) {
			$output = "";
			if(!isset($_input_charset) )$_input_charset  = $this->_input_charset ;
			if($_input_charset == $_output_charset || $input ==null ) {
				$output = $input;
			} elseif (function_exists("mb_convert_encoding")){
				$output = mb_convert_encoding($input,$_output_charset,$_input_charset);
			} elseif(function_exists("iconv")) {
				$output = iconv($_input_charset,$_output_charset,$input);
			} else die("sorry, you have no libs support for charset changes.");
			return $output;
		}
		
	/*********************************************************************************/
	}


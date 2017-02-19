<?php
/**
 * 微信统一下单
 * @param $payment 统一下单实体
 * @return array 根据支付客户端:PC返回二维码;APP返回支付参数
 * @throws Exception
 * @throws \WxPayException
 */
require_once 'weixin/WxPay.Data.php';
require_once 'weixin/WxPay.Api.php';
require_once 'weixin/WxPay.AppApiPay.php';
module_load_include('inc', 'orders', 'includes/config');
function unifiedorder($payment) {
  $weixin_settings = unserialize(variable_get('weixin_settings'));
  $input = new \WxPayUnifiedOrder();
  $input->SetBody($payment['body']);
  $input->SetAttach($payment['attach']);
  $input->SetOut_trade_no($payment['out_trade_no']);
  $input->SetTotal_fee($payment['total_fee']);
  $input->SetTime_start(date("YmdHis"));
  $input->SetTime_expire(date("YmdHis", time() + 600));
  // $input->SetGoods_tag($payment['good_tag']);
  $input->SetNotify_url($weixin_settings['notify_url']);
  //if ($_POST['clienttype'] == 'APP') {
  $input->SetTrade_type($payment['trade_type']);
  //} else {
  //  $input->SetProduct_id($payment['product_id']);
  //$input->SetTrade_type($weixin_settings['trade_type_native']);
  //}
  $order = \WxPayApi::unifiedOrder($input);
  $returnCode = $order['return_code'];
  $resultCode = $order['result_code'];
  if ($returnCode == 'SUCCESS') {
    if ($resultCode == 'SUCCESS') {
      //if ($_POST['clienttype'] == 'APP') {
      $tools = new \AppApiPay();
      $params = $tools->GetAppParameters($order);
      return ['data' => $params];
      echo json_encode(['errcode' => '10000', 'msg' => getConfig()['10000'], 'data' => $params]);
      exit;
      //} else {
      //  $tools = new \NativeApiPay();
      //$params = $tools->GetAppParameters($order);
      //return ['data' => $params];
      //echo json_encode(['errcode' => '10000','msg' => getConfig()['10000'],'data' => $params]);exit;
      //}

    }
    else {
      //echo json_encode([''])
      watchdog('weixin支付异常', $order['err_code'] . ":" . $order['err_code_des']);
      //$this->log->ERROR($order['err_code'].":".$order['err_code_des']);
      return services_error(print_r($order, TRUE), 406);
      echo json_encode(['errcode' => '10010', 'msg' => getConfig()['10010'], 'data' => []]);
      //throw new Exception($order['err_code'].":".$order['err_code_des'], Constant::WX_PAY_ERROR);
    }
  }
  else {
    watchdog('weixin支付异常', $order['return_msg']);
    return services_error(print_r($order, TRUE), 406);
    echo json_encode(['errcode' => '10011', 'msg' => getConfig()['10011'], 'data' => []]);
    //var_dump($order);
    //$this->log->ERROR($order['return_code'].":".$order['return_msg']);
    //throw new Exception($order['return_msg'], 1001);
  }
}

function refund($refund) {
  $weixin_settings = unserialize(variable_get('weixin_settings'));
  $input = new \WxPayRefund();
  $input->SetOut_trade_no($refund['trade_no']);
  $input->SetTotal_fee($refund['total_fee']);
  $input->SetRefund_fee($refund['fee']);
  $input->SetOut_refund_no($refund['sOrderNo']);
  $input->SetOp_user_id($weixin_settings['mchid']);
  $order = \WxPayApi::refund($input);
  $returnCode = $order['return_code'];
  $resultCode = $order['result_code'];
  if ($returnCode == 'SUCCESS') {
    if ($resultCode == 'SUCCESS') {
      return $order;
    }
    else {
      watchdog('weixin退款异常', $order['err_code'] . ":" . $order['err_code_des']);
      $this->log->ERROR($order['err_code'] . ":" . $order['err_code_des']);
      echo json_encode(['errcode' => '10016', 'msg' => getConfig()['10016'], 'data' => []]);
    }
  }
  else {
    watchdog('weixin退款异常', $order['return_code'] . ":" . $order['return_msg']);
    $this->log->ERROR($order['return_code'] . ":" . $order['return_msg']);
    echo json_encode(['errcode' => '10016', 'msg' => getConfig()['10016'], 'data' => []]);
  }
}

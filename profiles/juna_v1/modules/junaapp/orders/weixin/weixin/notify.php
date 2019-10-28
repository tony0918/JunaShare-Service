<?php
if (!defined("ROOT_PATH")) {
	define("ROOT_PATH", $_SERVER['DOCUMENT_ROOT'] . DIRECTORY_SEPARATOR);
}
//include_once '../../config/config.inc.php';
//require_once '../../config/sms.inc.php';
include_once 'WxPay.Notify.php';
include_once 'WxPay.Api.php';
require_once "log.php";

class WxPayNotifyCallBack extends \WxPayNotify {

	function __construct() {

	}

	//查询订单
	public function Queryorder($transaction_id) {
		$input = new WxPayOrderQuery();
		$input->SetTransaction_id($transaction_id);
		$result = WxPayApi::orderQuery($input);
		if (array_key_exists("return_code", $result)
			&& array_key_exists("result_code", $result)
			&& $result["return_code"] == "SUCCESS"
			&& $result["result_code"] == "SUCCESS"
		) {
			return true;
		}
		return false;
	}

	//重写回调处理函数
	public function NotifyProcess($data, &$msg) {
		watchdog('weixin_pay_notify', print_r($data, true));
		if (!array_key_exists("transaction_id", $data) || !array_key_exists("attach", $data)) {
			watchdog('weixin_pay_miss_args', 'miss transaction_id or attach');
			//$msg = "输入参数不正确";
			//$this->log->ERROR($msg);
			return false;
		}
		//查询订单，判断订单真实性
		if (!$this->Queryorder($data["transaction_id"])) {
			watchdog('weixin_pay_fail', 'query order fail');
			//$msg = "订单查询失败";
			//$this->log->ERROR($msg . ':' . $data["transaction_id"]);
			return false;
		}

		$attach = json_decode($data['attach']);
		//print_r($attach);exit;
		if ($attach->type == 'recharge') {
			$sOrderNo = isset($data['out_trade_no']) ? $data['out_trade_no'] : ''; //商户订单号
			//$sOrderId = isset($data['order_no']) ? $data['order_no'] : '';        //系统内业务订单号
			$tradeNo = isset($data['transaction_id']) ? $data['transaction_id'] : ''; //微信支付订单号
			$connStatus = isset($data['return_code']) ? $data['return_code'] : 'FAIL'; //通信状态
			$payStatus = isset($data['result_code']) ? $data['result_code'] : ''; //交易状态
			$totalFee = isset($data['total_fee']) ? $data['total_fee'] : 0; //支付金额
			try {
				if (strtoupper($connStatus) == 'SUCCESS') {
					//检查该账单是否已支付.....
					if (strtoupper($payStatus) == 'SUCCESS') {
						//支付成功
						$order = new stdClass();
						$order->id = $attach->orderid;
						$order->status = 2;
						$order->weixin_order_id = $tradeNo;
						$order -> payment_time = REQUEST_TIME;
						drupal_write_record('product_order', $order, 'id');
						$log = db_query('SELECT * from {user_amount_log} WHERE sorderid=:sorderid', array(':sorderid' => $sOrderNo))->fetchAll();
						if (empty($log)) {
							$record = new stdClass();
							$record->uid = $attach->uid;
							$record->total_fee = $attach->total_fee;
							$record->paytype = 'weixin';
							$record->paytime = REQUEST_TIME;
							$record->nid = $attach->nid;
							$record->status = 1;
							$record->sthirdorderno = $tradeNo;
							$record->type = "recharge";
							$record->sorderid = $sOrderNo;
							drupal_write_record('user_amount_log', $record);
						} else {
							db_update('user_amount_log')->fields(array('status' => 1, 'sthirdno' => $tradeNo))->condition('sorderid', $sOrderNo)->execute();
						}
						watchdog('weixin_recharge', "系统充值订单为:{$sOrderNo}的支付订单{$tradeNo}充值成功");
						return true;
					}
				} else {
					$log = db_query('SELECT * from {user_amount_log} WHERE sorderid=:sorderid', array(':sorderid' => $sOrderNo))->fetchAll();
					if (empty($log)) {
						$record = new stdClass();
						$record->uid = $attach->uid;
						$record->total_fee = $attach->total_fee;
						$record->paytype = 'weixin';
						$record->paytime = REQUEST_TIME;
						$record->nid = $attach->nid;
						$record->status = 2;
						$record->sthirdorderno = $tradeNo;
						$record->type = "recharge";
						$record->sorderid = $sOrderNo;
						drupal_write_record('user_amount_log', $record);
						watchdog('weixin_recharge', "系统充值订单为:{$sOrderNo}的支付订单{$tradeNo}充值失败,失败原因:{$data['return_code']}:{$data['return_msg']}");
					} else {
						db_update('user_amount_log')->fields(array('status' => 2, 'sthirdno' => $tradeNo))->condition('sorderid', $sOrderNo)->execute();
					}

					return false;
					watchdog('weixin_recharge', "系统充值订单为:{$sOrderNo}的支付订单{$tradeNo}充值失败,失败原因:{$data['return_code']}:{$data['return_msg']}");
					return false;
				}
				return true;
			} catch (Exception $e) {
				watchdog('weixin_pay_exeception', print_r($e, true));
				/*$where = array('sOrderNo' => $sOrderNo, 'iState' => Constant::RECHARGE_PAY_STATE_UNPROCESSED);
					$set = array('sThirdOrderNo' => $tradeNo);
					if ($e->getCode() == Constant::RECHARGE_PAY_ALL_FAILED) {
						$set['iState'] = Constant::RECHARGE_PAY_STATE_ALL_FAILED;
					} else {
						$set['iState'] = Constant::RECHARGE_PAY_CHANNEL_SUCCESS_STATE_FAILED;
						$log->error("系统充值订单为:{$sOrderNo}}的支付订单{$tradeNo}已支付完成,但系统充值失败!");
					}
					$this->fillCaseFlow->update($set, $where);
				*/
			}
			//$this->log->DEBUG(date('Y-m-d H:i:s', time()) . ":充值回调,订单号为:" . $attach->order_no);
			//watchdog('weixin_recharge');
			return true;
		} else if ($attach->type == "consume") {
			$sOrderNo = isset($data['out_trade_no']) ? $data['out_trade_no'] : ''; //商户订单号
			//$sOrderId = isset($data['order_no']) ? $data['order_no'] : '';        //系统内业务订单号
			$tradeNo = isset($data['transaction_id']) ? $data['transaction_id'] : ''; //微信支付订单号
			$connStatus = isset($data['return_code']) ? $data['return_code'] : 'FAIL'; //通信状态
			$payStatus = isset($data['result_code']) ? $data['result_code'] : ''; //交易状态
			$totalFee = isset($data['total_fee']) ? $data['total_fee'] : 0; //支付金额
			if (strtoupper($connStatus) == 'SUCCESS') {
				//检查该账单是否已支付.....
				if (strtoupper($payStatus) == 'SUCCESS') {
					//支付成功
					$order = new stdClass();
					$order->id = $attach->orderid;
					$order->status = 2;
					$order->weixin_order_id = $tradeNo;
					$order->payment_time = REQUEST_TIME;
					drupal_write_record('product_order', $order, 'id');
					$log = db_query('SELECT * from {user_amount_log} WHERE sorderid=:sorderid', array(':sorderid' => $sOrderNo))->fetchAll();
					if (empty($log)) {
						$record = new stdClass();
						$record->uid = $attach->uid;
						$record->total_fee = $attach->total_fee * 100;
						$record->paytype = 'weixin';
						$record->paytime = REQUEST_TIME;
						$record->nid = $attach->nid;
						$record->status = 1;
						$record->sthirdorderno = $tradeNo;
						$record->type = "conume";
						$record->sorderid = $sOrderNo;
						drupal_write_record('user_amount_log', $record);
					} else {
						db_update('user_amount_log')->fields(array('status' => 1, 'sthirdorderno' => $tradeNo))->condition('sorderid', $sOrderNo)->execute();
					}
					watchdog('weixin_consume', "系统充值订单为:{$sOrderNo}的支付订单{$tradeNo}充值成功");
					return true;
				}
			} else {
				$log = db_query('SELECT * from {user_amount_log} WHERE sorderid=:sorderid', array(':sorderid' => $sOrderNo))->fetchAll();
				if (empty($log)) {
					$record = new stdClass();
					$record->uid = $attach->uid;
					$record->total_fee = $attach->total_fee;
					$record->paytype = 'weixin';
					$record->paytime = REQUEST_TIME;
					$record->nid = $attach->nid;
					$record->status = 2;
					$record->sthirdorderno = $tradeNo;
					$record->type = "conume";
					$record->sorderid = $sOrderNo;
					drupal_write_record('user_amount_log', $record);
					watchdog('weixin_consume', "系统充值订单为:{$sOrderNo}的支付订单{$tradeNo}充值失败,失败原因:{$data['return_code']}:{$data['return_msg']}");
				} else {
					db_update('user_amount_log')->fields(array('status' => 2, 'sthirdno' => $tradeNo))->condition('sorderid', $sOrderNo)->execute();
				}

				return false;
			}
			return true;

		}
	}
}

<?php
require_once "WxPay.Api.php";
/**
 * 
 * APP支付返回参数封装类
 * 
 * @author jiafangyao
 *
 */
class AppApiPay
{
	public $data = null;


	public function GetAppParameters($UnifiedOrderResult)
	{
		if(!array_key_exists("appid", $UnifiedOrderResult)
			|| !array_key_exists("prepay_id", $UnifiedOrderResult)
			|| $UnifiedOrderResult['prepay_id'] == "")
		{
			throw new WxPayException("参数错误");
		}
		$appapi = new WxPayAppApiPay();
		$appapi->SetAppid($UnifiedOrderResult["appid"]);
		$appapi->SetPartnerid($UnifiedOrderResult["mch_id"]);
		$timeStamp = time();
		$appapi->SetTimeStamp("$timeStamp");
		$appapi->SetNonceStr(WxPayApi::getNonceStr());
		$appapi->SetPrepayId($UnifiedOrderResult['prepay_id']);
		$appapi->SetPackage('Sign=WXPay');
		$appapi->SetSign();
		$returnParam = $appapi->GetValues();
		$returnParam['packet'] = $appapi->GetPackage();
		unset($returnParam['package']);
		return $returnParam;
	}
}
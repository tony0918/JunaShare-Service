  <?php
require_once "WxPay.Api.php";
require_once LIB_PATH . "/vendor/qrcode/qrlib.php";

/**
 *
 * JSAPI支付实现类
 * 该类实现了从微信公众平台获取code、通过code获取openid和access_token、
 * 生成jsapi支付js接口所需的参数、生成获取共享收货地址所需的参数
 *
 * 该类是微信支付提供的样例程序，商户可根据自己的需求修改，或者使用lib中的api自行开发
 *
 * @author widy
 *
 */
class NativeApiPay {
	public $data = null;

	public function GetAppParameters($UnifiedOrderResult) {
		if (!array_key_exists("appid", $UnifiedOrderResult)
			|| !array_key_exists("prepay_id", $UnifiedOrderResult)
			|| $UnifiedOrderResult['prepay_id'] == ""
		) {
			throw new WxPayException("参数错误");
		}
		$appapi = new WxPayNativeApiPay();
		if (!is_dir(\WxPayConfig::QR_CODE_IMG_SAVE_PATH . 'qrcode')) {
			mkdir(\WxPayConfig::QR_CODE_IMG_SAVE_PATH . 'qrcode');
		}
		$relativePath = 'qrcode' . DIRECTORY_SEPARATOR . date('Ymd', time()) . DIRECTORY_SEPARATOR;
		$realPath = WxPayConfig::QR_CODE_IMG_SAVE_PATH . $relativePath;
		if (!is_dir($realPath)) {
			mkdir($realPath);
		}
		$filename = md5($_REQUEST['data'] . '|' . time() . '|H|10') . '.png';
		if (!empty($UnifiedOrderResult["code_url"])) {
			QRcode::png($UnifiedOrderResult["code_url"], $realPath . $filename, 'H', 10, 2);
		}

		$appapi->SetCodeurl(\WxPayConfig::QR_CODE_IMG_URL . $relativePath . $filename);
		return $appapi->GetValues();
	}
}
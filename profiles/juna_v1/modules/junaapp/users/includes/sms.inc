<?php
require_once 'alisms/TopSdk.php';
function sendsms($data) {
  //print_r($data);exit;
  $smssettings = unserialize(variable_get('smssettings'));
  $c = new TopClient;
  $c->format = 'json';
  $c->appkey = $smssettings['appkey'];//$appkey;
  $c->secretKey = $smssettings['secret'];//$secret;
  $req = new AlibabaAliqinFcSmsNumSendRequest;
  $req->setExtend("");
  $req->setSmsType("normal");
  $req->setSmsFreeSignName($smssettings['sign']);
  $param = json_encode(array('code' => $data['code'], 'product' => $smssettings['product']));
  $req->setSmsParam($param);
  $req->setRecNum($data['mobile']);
  $req->setSmsTemplateCode($smssettings['templateid']);//"SMS_585014"
  $resp = $c->execute($req);
  return $resp;
}

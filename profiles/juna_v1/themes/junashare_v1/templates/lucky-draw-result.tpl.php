<?php
$theme_path = url(drupal_get_path('theme', 'junashare_v1'));
?>
<div class="row bingo-product-info">
  <div class="col-xs-5 col-xs-offset-1">
    <p class="text-center">礼品总数：<?php echo $total_product; ?></p>
  </div>
  <div class="col-xs-5">
    <p class="text-center">参与人数：<?php echo $total_enrollment; ?></p>
  </div>
</div>
<div class="row">
  <div class="col-xs-8 col-xs-offset-2">
    <img class="img-responsive center-block" src="<?php echo $theme_path . '/images/bingo-list-icon.png' ?>" alt="">
  </div>
</div>
<div class="row bingo-users-info">
  <div class="col-xs-11 col-xs-offset-1">
    <?php foreach ($bingo_users as $key => $info): ?>
      <div class="clearfix bingo-user-item">
        <p class="bingo-user-index pull-left"><?php echo $key + 1; ?>.</p>
        <p class="bingo-user-number pull-left">摇享号：<?php echo $info['number']; ?></p>
        <p class="bingo-user-name pull-left"><?php echo $info['name']; ?></p>
        <p class="bingo-user-mobile pull-left"><?php echo $info['mobile']; ?></p>
      </div>
    <?php endforeach; ?>
  </div>
</div>

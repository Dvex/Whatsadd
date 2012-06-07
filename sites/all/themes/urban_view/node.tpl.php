<?php
// $Id: node.tpl.php $
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> post">
<div class="post-c">
<div class="post-cnt">
<?php print $picture ?>
<?php if ($submitted): ?>
    <small class="date">
	<?php print format_date($node->created, 'custom', "M");?>
	<div class="post-day"><?php print format_date($node->created, 'custom', "d");?></div>
	</small>
  <?php endif; ?>
<?php if ($page == 0): ?>
  <h2><span><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></span></h2>
<?php endif; ?>
  
  <div class="content clear-block">
    <?php print $content ?>
  </div>
</div>
</div>
  <div class="clear-block">
  <div class="post-b">
    <div class="meta post-meta post-cnt">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print 'Tags:' . $terms ?></div>
    <?php endif;?>
    </div>
    <?php if ($links): ?>
      <div class="links controls"><?php print $links; ?></div>
    <?php endif; ?>
    <div class="num-comments">
      <?php
      print $node->comment_count . ' comment';
      if ($node->comment_count != 1) { print 's'; } ?>
    </div>
  </div>
  </div>
  <div class="divider2"></div>

</div>

<?php
// $Id: node.tpl.php,v 1.3 2010/11/11 03:57:45 danprobo Exp $
?>
  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
    <?php if ($picture) {
      print $picture;
    }?>
    <?php if ($page == 0) { ?><h2 class="title"><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2><?php }; ?>
    <?php if ($submitted): ?><span class="submitted"><?php print $submitted; ?></span><?php endif; ?>
    <?php if ($taxonomy): ?><div class="taxonomy"><?php print $terms; ?></div><?php endif;?>
    <div class="content"><?php print $content; ?></div>
	<div style="clear:both"></div>
    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

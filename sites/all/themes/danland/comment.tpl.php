<?php
// $Id: comment.tpl.php,v 1.1.1.1 2010/04/06 03:11:41 danprobo Exp $
?>
  <div class="comment<?php print ' '. $status; ?>">
    <?php if ($picture) {
    print $picture;
  } ?>
<h3 class="title"><?php print $title; ?></h3><?php if ($new != '') { ?><span class="new"><?php print $new; ?></span><?php } ?>
    <div class="submitted"><?php print $submitted; ?></div>
    <div class="content">
     <?php print $content; ?>
     <?php if ($signature): ?>
      <div class="clear-block">
       <div>—</div>
       <?php print $signature; ?>
      </div>
     <?php endif; ?>
    </div>
    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

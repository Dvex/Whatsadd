<?php
// $Id: comment.tpl.php,v 1.1.2.1 2010/09/17 00:16:18 atelier Exp $
?>

<div class="comment <?php print $comment_classes;?> clear-block">
  <div class="comment-author">
		
    <?php if ($picture): ?>
			<?php print $picture ?>
    <?php endif; ?>
    
    <div class="author">
      <?php print $author ?>
    </div>
    
    <div class="submitted">
			<?php print format_date($comment->timestamp, 'custom', 'F j, Y'); ?><br />
      <?php print format_date($comment->timestamp, 'custom', 'g:h a'); ?>
    </div>
  
  </div>
  
  <div class="comment-content">
  
  	<h3 class="title"><?php print $title ?></h3>
  
    <div class="content">
      <?php print $content ?>
    </div>
      
  </div>
  
  <?php if ($signature): ?>
    <div class="signature">
      <?php print $signature ?>
    </div>
  <?php endif; ?>

	<?php if ($links): ?>
  	<div class="links">
    	<?php print $links ?>
  	</div>
  <?php endif; ?>
  
</div><!-- /comment -->
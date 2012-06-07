<?php
// $Id: block.tpl.php,v 1.3 2010/07/19 22:05:33 danprobo Exp $
?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block <?php print $block_classes; ?>">

<?php if (!empty($block->subject)): ?>
  <h2><?php print $block->subject ?></h2>
<?php endif;?>

  <div class="content"><?php print $block->content ?></div>
</div>

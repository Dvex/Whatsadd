<?php
// $Id: box.tpl.php,v 1.1 2010/09/11 20:29:12 atelier Exp $
?>

<div class="box">

<?php if ($title): ?>
  <h2 class="title"><?php print $title ?></h2>
<?php endif; ?>

  <div class="content"><?php print $content ?></div>
</div><!-- /box -->
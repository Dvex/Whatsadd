<?php
// $Id: page.tpl.php,v 1.9 2010/12/02 11:42:42 danprobo Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
    <meta http-equiv="Content-Style-Type" content="text/css" />
  <?php print $styles; ?>
   <!--[if IE 6]><link rel="stylesheet" href="<?php echo $base_path . $directory; ?>/style.ie6.css" type="text/css" /><![endif]-->
  <?php print $scripts; ?>
<!--[if IE 6]>
        <script type="text/javascript" src="<?php print $base_path . $directory; ?>/scripts/jquery.pngFix.js"></script>
<![endif]-->
<!--[if IE 6]>
<script type="text/javascript">
    jQuery(document).ready(function($) {
        $(document).pngFix();
    });
</script>
<![endif]-->
<script type="text/javascript">
  jQuery(document).ready(function($) {
    $("#superfish ul.menu").superfish({ 
            delay:       100,                           
            animation:   {opacity:'show',height:'show'},  
            speed:       'fast',                          
            autoArrows:  true,                           
            dropShadows: true                   
        });
  });
</script>
 </head>

<body<?php print phptemplate_body_class($left, $right); ?>>
<div id="header">
<div id="header-wrapper">
        <div id="header-first">
          <?php if ($logo): ?> 
          <div class="logo">
            <a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" height="50"/></a>
          </div>
          <?php endif; ?>
        </div><!-- /header-first -->
        <div id="header-middle">
	  <?php if ($site_name) : ?><h2 class="logo-name"><a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h2><?php endif; ?>
				<?php if ($site_slogan) : ?><div class='logo-text'><?php print $site_slogan; ?></div><?php endif; ?>
        </div><!-- /header-middle -->
        <div id="search-box">
          <?php print $search_box; ?>
        </div><!-- /search-box -->

	<div id="authorize">
      <ul><?php global $user; if ($user->uid != 0) { print '<li class="first">' .t('Logged in as '). '<a href="' .url('user/'.$user->uid). '">' .$user->name. '</a></li>'; print '<li><a href="' .url('logout'). '">' .t('Logout'). '</a></li>'; } else { print '<li class="first"><a href="' .url('user'). '">' .t('Login'). '</a></li>'; print '<li><a href="' .url('user/register'). '">' .t('Register'). '</a></li>'; } ?></ul>
	  <?php print $feed_icons; ?>
  </div>

      </div><!-- /header-wrapper -->

</div> <!-- /header -->
<div style="clear:both"></div>
<div id="menu">
<div id="rounded-menu-left"></div>
 <?php if ($primary_links || $superfish_menu): ?>
      <!-- PRIMARY -->
      <div id="<?php print $primary_links ? 'nav' : 'superfish' ; ?>">
        <?php 
					     if ($primary_links) {
		          print theme('links', $primary_links); 
				      }
				      elseif (!empty($superfish_menu)) {
				        print $superfish_menu;
				      }
        ?>
      </div> <!-- /primary -->
    <?php endif; ?>
<div id="rounded-menu-right"></div>
</div> <!-- end menu -->

 <?php if($preface_first || $preface_middle || $preface_last) : ?>
    <div style="clear:both"></div>
    <div id="preface-wrapper" class="in<?php print (bool) $preface_first + (bool) $preface_middle + (bool) $preface_last; ?>">
          <?php if($preface_first) : ?>
          <div class="column A">
            <?php print $preface_first; ?>
          </div>
          <?php endif; ?>
          <?php if($preface_middle) : ?>
          <div class="column B">
            <?php print $preface_middle; ?>
          </div>
          <?php endif; ?>
          <?php if($preface_last) : ?>
          <div class="column C">
            <?php print $preface_last; ?>
          </div>
          <?php endif; ?>
      <div style="clear:both"></div>
    </div>
    <?php endif; ?>

<div style="clear:both"></div>
<div id="wrapper">
<?php if ($left): ?>
			<div id="sidebar-left" class="sidebar">
				<?php print $left ?>
			</div>
		<?php endif; ?>
<div id="content">
			<?php if ($content_top) : ?><div class="content-top"><?php print $content_top; ?></div>
			<?php endif; ?>
			<?php if (!$is_front) print $breadcrumb; ?>
			<?php if ($show_messages) { print $messages; }; ?>
			<?php if ($tabs) : ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
			<?php if ($title) : ?><h1 class="title"><?php print $title; ?></h1><?php endif; ?>
			<?php print $help; ?>
		      <?php if ($content) : ?><div class="content-middle"><?php print $content; ?></div>
			<?php endif; ?>
			<?php if ($content_bottom) : ?><div class="content-bottom"><?php print $content_bottom; ?></div>
			<?php endif; ?>

</div> <!-- end content -->

<?php if ($right): ?>
			<div id="sidebar-right" class="sidebar">
				<?php print $right; ?>
			</div>
		<?php endif; ?>
<div style="clear:both"></div>
</div> <!-- end wrapper -->

<?php if($bottom_first || $bottom_middle || $bottom_last) : ?>
    <div style="clear:both"></div>
    <div id="bottom-teaser" class="in<?php print (bool) $bottom_first + (bool) $bottom_middle + (bool) $bottom_last; ?>">
          <?php if($bottom_first) : ?>
          <div class="column A">
            <?php print $bottom_first; ?>
          </div>
          <?php endif; ?>
          <?php if($bottom_middle) : ?>
          <div class="column B">
            <?php print $bottom_middle; ?>
          </div>
          <?php endif; ?>
          <?php if($bottom_last) : ?>
          <div class="column C">
            <?php print $bottom_last; ?>
          </div>
          <?php endif; ?>
      <div style="clear:both"></div>
    </div>
    <?php endif; ?>

 <?php if($bottom_1 || $bottom_2 || $bottom_3 || $bottom_4) : ?>
    <div style="clear:both"></div><!-- Do not touch -->
    <div id="bottom-wrapper" class="in<?php print (bool) $bottom_1 + (bool) $bottom_2 + (bool) $bottom_3 + (bool) $bottom_4; ?>">
          <?php if($bottom_1) : ?>
          <div class="column A">
            <?php print $bottom_1; ?>
          </div>
          <?php endif; ?>
          <?php if($bottom_2) : ?>
          <div class="column B">
            <?php print $bottom_2; ?>
          </div>
          <?php endif; ?>
          <?php if($bottom_3) : ?>
          <div class="column C">
            <?php print $bottom_3; ?>
          </div>
          <?php endif; ?>
          <?php if($bottom_4) : ?>
          <div class="column D">
            <?php print $bottom_4; ?>
          </div>
          <?php endif; ?>
      <div style="clear:both"></div>
    </div><!-- Bottom -->
    <?php endif; ?>

<div style="clear:both"></div>
<div id="footer-wrapper">
<div id="footer">
 <?php print $footer; ?>
</div>
<?php if($footer_message || $secondary_links) : ?>
<div id="subnav-wrapper">
 <ul><li><?php print $footer_message; ?></li>
<li><?php if (isset($secondary_links)) : ?><?php print theme('links', $secondary_links, array('class' => 'links', 'id' => 'subnav')); ?><?php endif; ?></li></ul>
</div>
<?php endif; ?>
</div> <!-- end footer wrapper -->

<div style="clear:both"></div>
<div id="notice"><p>Theme by <a href="http://www.danetsoft.com">Danetsoft</a> and <a href="http://www.danpros.com">Danang Probo Sayekti</a> inspired by <a href="http://www.maksimer.no">Maksimer</a></p></div>
<?php print $closure; ?>
</body>
</html>

// $Id: jquerymenu_no_animation.js,v 1.4 2010/05/05 07:50:55 aaronhawkins Exp $
Drupal.behaviors.jquerymenu = function(context) {
  $('ul.jquerymenu .active').parents('li').removeClass('closed').addClass('open');
  $('ul.jquerymenu .active').parents('li').children('span.parent').removeClass('closed').addClass('open');
  
    jqm_mouseenter = function() {
    momma = $(this);
    if ($(momma).hasClass('closed')){
      $(momma).removeClass('closed').addClass('open');
      $(this).removeClass('closed').addClass('open');
    }    
  }
  
  jqm_mouseleave = function(){
    momma = $(this);
    if ($(momma).hasClass('open')){
      $(momma).removeClass('open').addClass('closed');
      $(this).removeClass('open').addClass('closed');
    }
  }
 
  if (Drupal.settings.jquerymenu.hover === 1) {
    $('ul.jquerymenu:not(.jquerymenu-processed)', context).addClass('jquerymenu-processed').each(function(){
      $(this).find('li.parent').hover(jqm_mouseenter, jqm_mouseleave);
    });
    $('ul.jquerymenu-processed span.parent').remove();
  }

  else if (Drupal.settings.jquerymenu.hover === 0) { 
    $('ul.jquerymenu:not(.jquerymenu-processed)', context).addClass('jquerymenu-processed').each(function(){
      $(this).find("li.parent span.parent").click(function(){
        momma = $(this).parent();
        if ($(momma).hasClass('closed')){
          $(momma).removeClass('closed').addClass('open');
          $(this).removeClass('closed').addClass('open');
        }
        else{
          $(momma).removeClass('open').addClass('closed');
          $(this).removeClass('open').addClass('closed');
        }
      });
      showit = function() {
        $(this).children().show();
      }
      hideit = function() {
        $(this).children().hide();
      }
      $(this).find(".editbox").hover(showit, hideit);
    });
  }
}
// $Id: grunge-script.js,v 1.1 2010/09/11 20:28:39 atelier Exp $

Drupal.behaviors.grungeFirstword = function (context) {
  $('.block h2.title').each(function(){
      var bt = $(this);
      bt.html( bt.text().replace(/(^\w+)/,'<span class="first-word">$1</span>') );
  });
};

Drupal.behaviors.grungeSuperfish = function (context) {
  $("#primary-menu ul.sf-menu").superfish({
    hoverClass:  'sfHover',
    delay:       250,
    animation:   {opacity:'show',height:'show'},
    speed:       'fast',
    autoArrows:  true,
    dropShadows: false,
    disableHI:   true
  }).supposition();
};
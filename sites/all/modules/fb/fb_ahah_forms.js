
/**
 * The FBJS code here is based on ahah_forms.js.  This code is somewhat
 * limited by restrictions of Facebook Javascript.  For instance, we cannot
 * support the selector options of the original ahah_forms module.  Only where
 * the element and target are specified by id will this work.
 */

var Ahah = Ahah || {};

Ahah.update = function(e) {

  e.preventDefault(); // prevent click from submitting.
  var target = e.target;
  var element = target.fb_ahah_element;
  var wrapper = document.getElementById(element.wrapper);
  var imgSrc = Drupal.settings.fbjs.baseUrl + Drupal.settings.ahah.basePaths['module'] + '/lib/loading.gif';

  // let user know something is going on
  e.target.setDisabled(true);

  wrapper.setStyle('opacity', '0.3');

  var progress = document.createElement('div').setClassName('ahah_progress').setStyle({position: 'absolute', opacity: '1'});
  progress.setInnerXHTML('<img src="' + imgSrc + '" />');
  wrapper.insertBefore(progress);

  var uri = Drupal.settings.fbjs.baseUrlFb + element.path;

  var ajax = new Ajax();
  ajax.responseType = Ajax.RAW;
  ajax.onerror = function() {
    new Dialog().showMessage("onerror called!", 'foo');
  };

  ajax.ondone = function(data) {
    // Can't use e.target here.
    target.setDisabled(false);
    wrapper.setStyle({opacity: '1'});

    wrapper.setInnerXHTML(data);
    //new Dialog().showMessage("ondone called!", data);

    // In case the new data includes AHAH elements, we need to process them again
    Ahah.attach_all_bindings();
  };

  // TODO: make requireLogin dynamic
  //ajax.requireLogin = false;
  ajax.post(uri);
};


Ahah.attach_to_element = function(element) {
  // The original ahah_forms supported versatile selectors.  In facebook, we can only find elements by id.
  if (element.id) {
    var elem_id = element.id;
    var elem = document.getElementById(elem_id);
    if (!elem.fb_ahah_element) {
      // Store data that we'll need during the event.
      elem.fb_ahah_element = element;
      // Make sure we are called during the event.
      elem.addEventListener(element.event, Ahah.update);
    }
  }
};

/**
 *  Attach listeners to all elements
 */
Ahah.attach_all_bindings = function( ) {
  var element;
	// Drupal.ahah.elements is an array of arrays of elements
  for (var i in Drupal.settings.ahah.bindings ) {
    for (var j in Drupal.settings.ahah.bindings[i] ) {
      if (!isNaN(j)) {
        element = Drupal.settings.ahah.bindings[i][j];
        if (element) {
          Ahah.attach_to_element( element );
        }
      }
    }
  }
};


Ahah.attach_all_bindings();
//new Dialog().showMessage("fbjs is alive", "foo");


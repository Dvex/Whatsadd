Drupal for Facebook
-------------------

More information:
http://www.drupalforfacebook.org, http://drupal.org/project/fb

Primary author and maintainer: Dave Cohen (http://www.dave-cohen.com/contact)
Do  NOT contact  the  maintainer  with a question  that  can be  easily
answered with a web search.  You may not receive a reply.

Branch: 6.x-3.x (version 3.x for Drupal 6.x)

This file is more current than online documentation.  When in doubt,
trust this file.  Online documentation: http://drupal.org/node/195035,
has more detail and you should read it next..

To upgrade:

- Read the upgrade instructions: http://drupal.org/node/936958

To install:

- Make sure you have a PHP client from facebook (version >= 3.1.1).
  The 2.x.y versions are not supported by this version of Drupal for Facebook.
  Download from http://github.com/facebook/php-sdk.
  Extract the files, and place them in sites/all/libraries/facebook-php-sdk.

  If you have the Libraries API module installed, you may place the files in
  another recognised location (such as sites/all/libraries), providing that the
  directory is named 'facebook-php-sdk'.

  Or, to manually set the location of the php-sdk in any other
  directory, edit your settings.php to include a line similar to the
  one below. Add to the section where the $conf variable is defined,
  or the very end of settings.php. And customize the path as needed.

  $conf['fb_api_file'] = 'sites/all/libraries/facebook-php-sdk/src/facebook.php';

  See also http://drupal.org/node/923804

- Your theme needs the following attribute at the end of the <html> tag:

  xmlns:fb="http://www.facebook.com/2008/fbml"

  Typically, this means editing your theme's page.tpl.php file.  See
  http://www.drupalforfacebook.org/node/1106.  Note this applies to
  themes used for Facebook Connect, iframe Canvas Pages, and Social
  Plugins (i.e. like buttons).  Without this attribute, IE will fail.

  Note that some documention on facebook.com suggests
  xmlns:fb="http://ogp.me/ns/fb#" instead of the URL above.  Try that
  if the above is not working for you.


- To support canvas pages and/or page tabs, url rewriting and other
  settings must be initialized before modules are loaded, so you must
  add this code to your settings.php.  This is done by adding these
  two lines to the end of sites/default/settings.php (or
  sites/YOUR_DOMAIN/settings.php).

  include "sites/all/modules/fb/fb_url_rewrite.inc";
  include "sites/all/modules/fb/fb_settings.inc";

  (Change include paths if modules/fb is not in sites/all.)

- For canvas pages, add something like this to your settings.php:

  if (!headers_sent()) {
    header('P3P: CP="We do not have a P3P policy."');
  }

  See http://drupal.org/node/933994 and search for "P3P" for details.

- Go to Administer >> Site Building >> Modules and enable the Facebook
  modules that you need.

  Enable fb.module for Social Plugins.

  Enable fb_devel.module and keep it enabled until you have everything
  set up.  You should disable this on your live server once you are
  certain facebook features are working.  (Note this requires
  http://drupal.org/project/devel, which is well worth installing
  anyway.)

  Enable fb_app.module and fb_user.module if you plan to create
  facebook applications.

  Enable fb_connect.module for Facebook Connect and/or
  fb_canvas.module for Canvas Page apps.

  Pages at http://drupal.org/node/932690 will help you decide which
  other modules you need to enable for your particular needs.


To support Facebook Connect, Canvas Pages, and/or Social Plugins that
require an Application, read on...

- You must enable clean URLs.  If you don't, some links that drupal
  creates will not work properly on canvas pages.

- Create an application on Facebook, currently at
  http://www.facebook.com/developers/editapp.php?new.  Fill in the
  minimum required to get an apikey and secret.  If supporting canvas
  pages, specify a canvas name, too.  You may ignore other settings
  for now.

- Go to Administer >> Site Building >> Facebook Applications and click
  the Add Applicaiton tab.  Use the app id, apikey and secret that
  Facebook has shown you.  Hopefully other settings will be
  self-explanitory.  When you submit your changes, Drupal for Facebook
  will automatically set the callback URL and some other properties
  which help it work properly.


Troubleshooting:
---------------

Reread this file and follow instructions carefully.

Read http://drupal.org/node/933994, and all the module documentation
on http://drupal.org/node/912614.

Enable the fb_devel.module and add the block it provides (called
"Facebook Devel Page info") to the footer of your Facebook theme.
fb_devel.module will catch some errors and write useful information to
Drupal's log and status page.

Use your browser's view source feature, and search page source for any
<script> tag which includes facebook's javascript,
"http://connect.facebook.net/en_US/all.js".  fb.js will include this
for you.  Including it too soon will break many features.  So remove
it from any block, node, template or whatever that adds it to the
page.  Similarly, do not include any <div id="fb-root">.

Disable Global Redirect, if you have that module installed.  Users
have reported problems with it and Drupal for Facebook.  Any module
which implements custom url rewrites could interfere with canvas page
and profile tab support.

On the facebook side, make sure your application is not in "sandbox
mode".  This is known to have unwanted side effects.  Also, don't use
a test account. If you've used a test account, ever, even for another
application, clear all your browser's cookies.  Try to reproduce the
problem not in sandbox mode, and not using a test account.

Bug reports and feature requests may be submitted.
Here's an idea: check the issue queue before you submit
http://drupal.org/project/issues/fb

If you do submit an issue, start the description with "I read the
README.txt from start to finish," and you will get a faster, more
thoughtful response.  Seriously, prove that you read this far.

Below are more options for your settings.php.  Add the PHP shown below
to the very end of your settings.php, and modify the paths accordingly
(i.e. where this example has "sites/all/modules/fb", you might need
"profiles/custom/modules/fb").




//// Code to add to settings.php:
/////////////////////////////////

/**
 * Drupal for Facebook settings.
 */

if (!is_array($conf))
  $conf = array();

$conf['fb_verbose'] = TRUE; // debug output
//$conf['fb_verbose'] = 'extreme'; // for verbosity fetishists.

// More efficient connect session discovery.
// Required if supporting one connect app and different canvas apps.
//$conf['fb_id'] = '123.....XYZ'; // Your connect app's ID goes here.

// Enable URL rewriting (for canvas page apps).
include "sites/all/modules/fb/fb_url_rewrite.inc";
include "sites/all/modules/fb/fb_settings.inc";

// Header so that IE will accept cookies on canvas pages.
if (!headers_sent()) {
  header('P3P: CP="We do not have a P3P policy."');
}

// end of settings.php

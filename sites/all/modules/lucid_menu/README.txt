
Lucid menu is a highly customizable module which replaces the current menus with new ones. It utilizes the superfish project to achieve extremely eloquent events.

1. Installation
   a. Install the latest stable version of jquery_update
   b. Simply enable the module
   
2. Go to http://users.tpg.com.au/j_birch/plugins/superfish/#download, download the latest superfish (this module was tested with 1.4.8). Rename superfish-<version> folder that results from extraction to superfish and move it to the module's root folder. Its path relative to the module root should now be lucid_menu/superfish.
   
3. Configuration
   Visit "Administer"->"Site configuration"->"Lucid menu" to configure the global settings, the defaults are recommended
   
4. A new block will be created for every menu block, enable the block in place of the original one and enjoy! You can configure the block for block specific options.

5. For theming, there are three main css files responsible for theming

   superfish/css/superfish.css (essential + horizontal)
   superfish/css/superfish-vertical.css (vertical)
   superfish/css/superfish-navbar.css (navbar)
   
   The module will look for the files in this order for overrides:
   	<path-to-theme>/superfish-<menu number>.css (you can get the menu number from the block listing page)
   	<path-to-theme>/superfish.css
   	<lucid-menu-module>/superfish/css/superfish.css
   	
   Same override applies for superfish-vertical.css and superfish-navbar.css

Please use the project's page to report any bugs, suggestions or support requests.

For paid customization of the module or drupal consultation, please contact me at mistknight@gmail.com

http://aamayreh.org
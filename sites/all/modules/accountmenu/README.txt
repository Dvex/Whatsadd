
This modules provides a menu with links for [Log in][My account][Log out].

These links are dynamic, only [Log in] is displayed if the user has not
logged in. After, [My account][Log in] are shown.

There is a "Register" link that's initially disable. Enable this if you want
to have one link for login and one for register. You can customize the menu
link title by editing the menu.

By default these links are in the menu "Account menu". It can be used like
the primary-links menu. These links can be moved to be part of any other
menu using the admin/settings/accountmenu screen.

The [Log in] link takes the user to the Drupal login screen. After the user
sucessfully log in, the screen return back to the page from where the user
originally click [Log in] link.

The links are fully configurable through the admin/build/menu interface.
However, remember that if you subsequently move the links to another menu,
the configuration changes are lost. So it's best to first move the links
first, then set configuration.

The "My account" link title can include tokens @name or @realname. @name is replaced
with the log in name, @realname is replace with the real name if the RealName module
is installed, if not, it's replaced with the log in name.

The accountmenu block title can have token @name and @realname and these works
the same as the "My account" link title
// $Id: README.txt,v 1.10.2.2 2011/01/04 20:57:50 vikramy Exp $
NODE EXPIRE
===========

This module allows you to set a "timer" into content nodes. When it reaches zero,
you can perform any type of action with the node, such as unpublishing it or
sending an email to the author.

All this power is possible due Rules module. On each cron, Node expire scan for
expired content and let Rules module work with it. You can select several actions
to perform with these nodes.

If using jQuery UI module, the date field will activate a Calendar widget in order
to make the process easier and more fun.

If using Views module, all data will be exported, allowing you to build custom lists.


INSTALL
=======

This module is not (YET at least) compatible with previous versions. So its only
indicated to new sites.


USAGE
=====

The first thing you should do is give the proper permissions: "administer node expire"
will allow you to enable the feature on node types and put a default value. "edit
node expire" will allow you to put the real date on nodes.

Then you should go to admin/content/types and should the node type that will use
expiration feature. Under "Workflow settings", put the default expiration date
using PHP strtotime format.

Now, all users that have "edit node expire" will be able to select a different
expiration date during node creation/editing. If not, the default value will be
used. Note that if the user edit the node, the expiration date will not change.


CREDITS
=======

Daryl Houston     <daryl@learnhouston.com> (Original author)
Andrew Langland                            (D5-dev and D6-dev rewrite)
Bruno Massa                                (D6 v2 rewrite)


RULES MODULE EXAMPLE
====================

For those people that want to use this module quickly, import the code below on
admin/rules/ie/import and it will automatically configure it to unpublish the
content once it expires. Just paste it and have fun.

array (
  'rules' =>
  array (
    'rules_6' =>
    array (
      '#type' => 'rule',
      '#set' => 'event_node_expired',
      '#label' => 'Content expired',
      '#active' => 1,
      '#weight' => '0',
      '#status' => 'custom',
      '#conditions' =>
      array (
      ),
      '#actions' =>
      array (
        0 =>
        array (
          '#weight' => 0,
          '#info' =>
          array (
            'label' => 'Unpublish content expired',
            'module' => 'Node',
            'arguments' =>
            array (
              'node' =>
              array (
                'label' => 'Content',
                'type' => 'node',
              ),
            ),
            'base' => 'rules_core_action_execute',
            'action_name' => 'node_unpublish_action',
            'configurable' => false,
            'label callback' => 'rules_core_node_label_callback',
            'label_skeleton' => 'Unpublish @node',
          ),
          '#name' => 'rules_core_node_unpublish_action',
          '#settings' =>
          array (
            'auto_save' => 1,
            '#argument map' =>
            array (
              'node' => 'node',
            ),
          ),
          '#type' => 'action',
        ),
      ),
    ),
  ),
)
--------------------------------------------------------------------------------------------
"Send email remainders every two weeks".

array (
  'rules' =>
  array (
    'rules_11' =>
    array (
      '#type' => 'rule',
      '#set' => 'event_node_expired',
      '#label' => 'Node expire',
      '#active' => 1,
      '#weight' => '0',
      '#categories' =>
      array (
      ),
      '#status' => 'custom',
      '#conditions' =>
      array (
        0 =>
        array (
          '#type' => 'condition',
          '#settings' =>
          array (
            '#argument map' =>
            array (
              'node' => 'node',
            ),
          ),
          '#name' => 'node_expire_rules_expired_check_lastnotify',
          '#info' =>
          array (
            'arguments' =>
            array (
              'node' =>
              array (
                'type' => 'node',
                'label' => 'Content',
              ),
            ),
            'label' => 'Content is expired: Check lastnotify',
            'module' => 'Node',
          ),
          '#weight' => 0,
        ),
      ),
      '#actions' =>
      array (
        0 =>
        array (
          '#weight' => 0,
          '#info' =>
          array (
            'label' => 'Send a mail to a user',
            'arguments' =>
            array (
              'user' =>
              array (
                'type' => 'user',
                'label' => 'Recipient',
              ),
            ),
            'module' => 'System',
            'eval input' =>
            array (
              0 => 'subject',
              1 => 'message',
              2 => 'from',
            ),
          ),
          '#name' => 'rules_action_mail_to_user',
          '#settings' =>
          array (
            'from' => '',
            'subject' => 'Remainder email',
            'message' => 'Email message',
            '#argument map' =>
            array (
              'user' => 'author',
            ),
          ),
          '#type' => 'action',
        ),
        1 =>
        array (
          '#type' => 'action',
          '#settings' =>
          array (
            '#argument map' =>
            array (
              'node' => 'node',
            ),
          ),
          '#name' => 'node_expire_update_lastnotify',
          '#info' =>
          array (
            'arguments' =>
            array (
              'node' =>
              array (
                'type' => 'node',
                'label' => 'content expired',
              ),
            ),
            'label' => 'Update lastnotify',
            'module' => 'Node',
          ),
          '#weight' => 0,
        ),
      ),
      '#version' => 6003,
    ),
  ),
)

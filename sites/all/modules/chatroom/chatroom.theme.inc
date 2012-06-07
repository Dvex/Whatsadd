<?php

/**
 * $file
 * Theme functions for chatroom.module.
 */

/**
 * Implementation of hook_theme().
 */
function chatroom_theme($existing, $type, $theme, $path) {
  // Add the path to our module folder so we can pick up popout template.
  $page = $existing['page'];
  $page['theme paths'][] = drupal_get_path('module', 'chatroom');
  return array(
    'page' => $page,
    'chatroom_message_username' => array(
      'arguments' => array('message', 'skip_permission_check'),
    ),
    'chatroom_chat_max_users' => array(
      'arguments' => array('node'),
    ),
    'chatroom_chat_admin_buttons' => array(
      'arguments' => array('chat'),
    ),
    'chatroom_chat_user_link' => array(
      'arguments' => array('link'),
    ),
    'chatroom_allowed_users_list' => array(
      'arguments' => array('allowed_users'),
    ),
    'chatroom_chat_make_private_button' => array(
      'arguments' => array('node'),
    ),
    'chatroom_allowed_user' => array(
      'arguments' => array('allowed_user'),
    ),
    'chatroom_block_chats' => array(
      'arguments' => array(),
    ),
    'chatroom_latest_message' => array(
      'arguments' => array('message'),
    ),
    'chatroom_block_chatrooms' => array(
      'arguments' => array(),
    ),
    'chatroom_desk' => array(
      'arguments' => array('open_message', 'closed_message', 'staff'),
    ),
    'chatroom_desk_staff' => array(
      'arguments' => array('rooms'),
    ),
    'chatroom_message' => array(
      'arguments' => array('message', 'node'),
    ),
    'chatroom_system_message' => array(
      'arguments' => array('message', 'node'),
    ),
    'chatroom_user_list' => array(
      'arguments' => array('users', 'node'),
    ),
    'chatroom_chat_kicked_user' => array(
      'arguments' => array('node'),
    ),
    'chatroom_chat_banned_user' => array(
      'arguments' => array('node'),
    ),
    'chatroom_list' => array(
      'arguments' => array('tree'),
    ),
    'chatroom_chat' => array(
      'arguments' => array('node'),
    ),
    'chatroom_chat_archive' => array(
      'arguments' => array('node'),
    ),
    'chatroom_buttons' => array(
      'arguments' => array('node'),
    ),
    'chatroom_chat_teaser' => array(
      'arguments' => array('node'),
    ),
    'chatroom_teaser' => array(
      'arguments' => array('node'),
    ),
  );
}

/**
 * Theme the site-wide chat list block.
 *
 * @ingroup themeable
 */
function theme_chatroom_block_chats() {
  $content = '<ul class="menu" id="chatroom-sitewide-chats">';
  if ($chats = chatroom_get_active_chats()) {
    $total = variable_get('chatroom_block_chats', 5);
    foreach ($chats as $number => $node) {
      if ($number == $total) {
        break;
      }
      $id = "chatroom-block-chats-$node->nid";
      $content .= '<li id="' . $id .'">'. l($node->title, "node/$node->nid");
      if (isset($chat->chatroom)) {
        $room_link = l($node->chatroom->title, "node/$node->chatroom->nid");
        $content .= '<br /><span class="chatroomLink">'. t('in') .' '. $room_link .'</span>';
      }
      $content .= '</li>';
    }
  }
  else {
    $content .= '<li id="chat_empty"><em>'. t('There are no active chats.') .'</em></li>';
  }
  $content .= '</ul>';
  return array(
    'content' => $content,
    'subject' => t('Active chats'),
  );
}

/**
 * Theme the chat room list block.
 *
 * @ingroup themeable
 */
function theme_chatroom_block_chatrooms() {
  $content = '<ul class="menu" id="chatroom-sitewide-chatrooms">';
  $rooms = chatroom_get_active_chatrooms();
  if (empty($rooms)) {
    $content .= '<li id="chatroom_empty"><em>'. t('There are no active chat rooms.') .'</em></li>';
  }
  else {
    $total = variable_get('chatroom_block_chatrooms', 5);
    foreach ($rooms as $number => $room) {
      if ($number == $total) {
        break;
      }
      $content .= '<li id="chatroom_'. $room->nid .'">'. l($room->title, "node/$room->nid") .'</li>';
    }
  }
  $content .= '</ul>';
  return array(
    'content' => $content,
    'subject' => t('Active chat rooms'),
  );
}

/**
 * Provide a block helpful to users who want to chat with the help desk.
 * @param string $open_message
 * @param string $closed_message
 * @param array $staff
 * @return string
 * @ingroup themeable
 */
function theme_chatroom_desk($open_message, $closed_message, $staff){
  global $user;
  if ($user->uid && (!user_access('access chat rooms') || !user_access('use chat desk'))) {

    // If they are logged in and not allowed to chat, nothing to see.
    return array(
      'content' => '',
      'subject' => '',
    );
  }
  // Show the message
  if (empty($staff)) {
    $content = '<div class="chatroom_desk_closed">'. $closed_message .'</div>';
  }
  else {
    $content = '<div class="chatroom_desk_open">'. $open_message .'</div>';
    $users_chats = chatroom_get_users_active_chats();
    if (empty($users_chats)) {
      $content .= drupal_get_form('chatroom_desk_open_form');
    }
    else {
      $content .= "<p>". t('Your current chat:') ."</p>";
      foreach ($users_chats as $chat) {
        $items[] = l($chat->title, 'node/'. $chat->nid);
      }
      $content .= theme('item_list', $items);
    }
  }
  return array(
    'content' => $content,
    'subject' => t('Live Chat Desk'),
  );
}

/**
 * Return a block useful for for help desk staff.
 * @return string
 *
 * @ingroup themeable
 */
function theme_chatroom_desk_staff($rooms){
  // Show the message.
  if (empty($rooms)) {
    $content = '<div class="chatroom_desk_empty">'. t('There are no active help desk chatrooms.') .'</div>';
  }
  else {
    $content = '<div class="chatroom_desk_active">'. t('There are active help desk chatrooms.') .'</div>';
    foreach ($rooms as $chat) {
      $items[] = l($chat->title, 'node/'. $chat->nid);
    }
    $content .= theme('item_list', $items);
  }
  return array(
    'content' => $content,
    'subject' => t('Active chat desk rooms'),
  );
}

/**
 * Format an array of messages.
 *
 * @ingroup themeable
 */
function theme_chatroom_messages(array $messages, $node) {
  $output = '';
  foreach($messages as $message) {
    $output .= chatroom_chat_get_themed_message($message, $node);
  }
  return $output;
}

/**
 * Format a single message for display.
 *
 * @param mixed $message
 * @param mixed $node
 * @return string
 */
function theme_chatroom_message($message, $node) {
  return check_markup($message->msg, $node->format, FALSE);
}

/**
 * Format a single system message for display.
 *
 * @param mixed $message
 * @param mixed $node
 * @return string
 */
function theme_chatroom_system_message($message, $node) {
  return '<div class="chatroom-system-msg new-message">' . check_markup($message, $node->format, FALSE) . '</div>';
}

/**
 * Format the list of users in a chatroom.
 *
 * @param mixed $users
 * @param mixed $node
 * @return string
 */
function theme_chatroom_user_list($users, $node) {
  global $user;
  $is_admin = $user->uid == $node->uid || user_access('administer chats');

  // Get the links to be shown after the user name.
  $user_links = chatroom_chat_get_user_links($node, $users);
  $active_user_links = chatroom_chat_get_active_user_links($node);

  $html = '<div id="chatroom-user-list-wrapper"><ul id="chatroom-user-list">';
  $preset = FALSE;
  if (module_exists('imagecache') && $node->chat->imagecache_preset) {
    $preset = imagecache_preset_by_name($node->chat->imagecache_preset);
  }
  foreach ($users as $chat_user) {
    $class = 'chatroom-user' . ($chat_user->sid == session_id() ? ' chatroom-current-user' : '');
    $id = 'chatroom_' . ($chat_user->uid ? "user_$chat_user->uid" : "guest_$chat_user->guest_id");
    $html .= '<li id="' . $id . '" class="' . $class . '">';
    if ($chat_user->uid && $node->chat->profile_picture && $chat_user->picture) {
      if ($preset) {
        $alt = t("@user's picture", array('@user' => $chat_user->name));
        $chat_user->picture = theme('imagecache', $preset['presetname'], $chat_user->picture, $alt, $alt);
        $html .= $chat_user->picture . theme('username', $chat_user);
      }
      else {
        $html .= theme('user_picture', $chat_user);
      }
    }
    else {
      $html .= theme('username', $chat_user);
    }

    // Add links after the user name.
    $visible_links = array();
    if ($chat_user->uid == $user->uid) {
      foreach ($active_user_links as $link) {
        if ($link->admin_only && !$is_admin) {
          continue;
        }
        $visible_links[] = theme('chatroom_chat_user_link', $link);
      }
    }
    foreach ($user_links as $link) {
      if ($link->admin_only && !$is_admin) {
        continue;
      }
      if ($chat_user->uid == $user->uid && !$link->show_active_user) {
        continue;
      }
      $visible_links[] = theme('chatroom_chat_user_link', $link);
    }
    $html.= ' ' . implode(' ', $visible_links) . '</li>';
  }
  $html .= '</ul></div>';
  return $html;
}

/**
 * Format a message for display in a summary table.
 */
function theme_chatroom_latest_message($message) {
  $output = check_plain($message->msg) . '<br />';
  $output .= t('Posted by <strong>!user</strong> on !date', array(
    '!user' => $message->uid ? l($message->name, "user/$message->uid") : variable_get('chatroom_guest_user_prefix', t('guest-')) . $message->guest_id,
    '!date' => format_date($message->modified, 'medium'),
  ));
  return $output;
}

/**
 * Format the chat room table.
 *
 * @ingroup themeable
 */
function theme_chatroom_list($rooms) {
  global $user;

  if ($rooms) {
    $header = array(t('Chat room'), t('Chats'), t('Messages'), t('Last message'));
    foreach ($rooms as $room) {
      $description  = "<div>\n";
      $description .= ' <div class="name">'. l($room->title, "node/$room->nid") .'</div>';

      if ($room->body) {
        $description .= ' <div class="description">'. check_markup($room->body, $room->format, FALSE) ."</div>\n";
      }
      $description .= "</div>\n";

      $rows[] = array(array('data' => $description, 'class' => 'container', 'colspan' => '4'));

      if (isset($room->chatroom->chats)) {
        foreach ($room->chatroom->chats as $id => $chat) {
          $description  = "<div>\n";
          $description .= ' <div class="name">'. l($chat->chatname, "chatroom/chat/$chat->ccid") ."</div>\n";
          $description .= "</div>\n";

          $row[] = array('data' => '&nbsp;');
          $row[] = array('data' => $description, 'class' => 'chatroom-chat');
          $row[] = array('data' => $chat->msg_count, 'class' => 'chatroom-msg-count');
          $row[] = array('data' => $chat->msg_info, 'class' => 'chatrom-msg-info');
          $rows[] = $row;
          unset($row);
        }
      }
    }
    return theme('table', $header, $rows);
  }
}

/**
 * Get HTML for kick message.
 *
 * @ingroup themeable
 */
function theme_chatroom_chat_kicked_user($node) {
  $msg = $node->chat->kicked_out_message ? $node->chat->kicked_out_message : t('You have been kicked out of %chat for misbehaving.', array('%chat' => $node->title));
  return '<div id="chatroom-kicked-msg">'. $msg .'</div>';
}

/**
 * Get HTML for ban message.
 *
 * @ingroup themeable
 */
function theme_chatroom_chat_banned_user($node) {
  $msg = $node->chat->banned_message ? $node->chat->banned_message : t('You have been banned from %chat for misbehaving.', array('%chat' => $node->title));
  return '<div id="chatroom-banned-msg">'. $msg .'</div>';
}

/**
 * Theme a chat.
 *
 * @ingroup themeable
 */
function theme_chatroom_chat($node) {
  global $user;

  $output = '<style type="text/css">#chatroom-board-container { width: ' . variable_get('chatroom_message_board_width', '100%') . ';}</style>';
  $output .= theme('chatroom_user_list', $node->chat->users, $node);
  $output .= '<div id="chatroom-board-container">';
  $output .= '<div id="chatroom-board" role="log">';
  foreach ($node->chat->latest_messages as $message) {
    $output .= chatroom_chat_get_themed_message($message, $node);
  }
  $output .= '</div></div>';
  $output .= theme('chatroom_buttons', $node);
  if ($node->uid == $user->uid || user_access('administer chats')) {
    $output .= drupal_get_form('chatroom_chat_management_form', $node);
  }
  return $output;
}

/**
 * Theme the make private button.
 *
 * @param mixed $node
 * @return string
 */
function theme_chatroom_chat_make_private_button($node) {
  return '<div id="chatroom-make-private-button"><a href="#make-private-button">[' . t("Make this chat private") . ']</a></div>';
}

/**
 * Form fields for chatroom text entry.
 *
 * @param mixed $form_state
 * @param mixed $node
 * @return array
 */
function chatroom_chat_buttons($form_state, $node) {
  global $user;
  if (!$user->uid && variable_get('chatroom_allow_anon_name', TRUE)) {
    $form['chatroom_anon_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter your name'),
      '#size' => 20,
      '#maxlength' => 256,
    );
    $form['form_token'] = array(
      '#type' => 'hidden',
      '#value' => drupal_get_token('chatroom_chat_buttons'),
      '#id' => 'edit-chatroom-chat-buttons-form-token',
    );
  }
  $form['chatroom_message_entry_box'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter your message text here'),
    '#size' => 50,
    '#rows' => 1,
    '#maxlength' => variable_get('chatroom_max_message_size', 1000),
  );
  $form['chatroom_message_entry_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Chat'),
  );
  return $form;
}

/**
 * Theme chat buttons.
 *
 * @ingroup themeable
 */
function theme_chatroom_buttons($node) {
  $output = drupal_get_form('chatroom_chat_buttons', $node);
  return '<div id="chatroom-chat-buttons">' . $output . '</div>';
}

/**
 * Theme the chat archive page.
 *
 * @ingroup themeable
 */
function theme_chatroom_chat_archive($node) {
  global $user;

  $is_admin = user_access('administer chats') || user_access('administer chat rooms');

  $html = '<p>' . t('Archived on !date.', array('!date' => format_date($node->chat->when_archived, 'medium'))) . '</p>';
  $html .= '<div id="chatroom-container-archive">';
  if ($messages = chatroom_chat_load_messages($node->nid)) {
    foreach ($messages as $message) {
      if ($message->uid) {
        $name = l($message->name, 'user/'. $message->uid);
      }
      else {
        $name = variable_get('chatroom_guest_user_prefix', t('guest-')) . $message->guest_id;
      }
      if (empty($message->recipient_uid)) {
        $row_html = '<p class="chatroom-old-msg">' . format_date($message->modified, 'small') . " $name: " . check_plain($message->msg) . "</p>";
      }
      else if ($is_admin || $user->uid == $message->uid || $user->uid == $message->recipient_uid) {
        $row_html = '<p class="chatroom-old-msg">' . format_date($message->modified, 'small');
        if ($message->uid == $user->uid && $recipient != $name) {
          $row_html .= '(' . t('privately to %recipient', array('%recipient' => $recipient)) . ')> ';
        }
        else {
          $row_html .= '(' . t('privately') . '):';
        }
        $row_html .= check_plain($message->msg) . '</p>';
      }
      $html .= "$row_html\n";
    }
  }
  else {
    $html .= '<p>' . t('This chat contains no messages.') . '</p>';
  }
  $html .= '</div>';
  return $html;
}

/**
 * Implementation of hook_preprocess_page().
 */
function chatroom_preprocess_page(&$vars) {
  // Add a template hint for the popout page template.
  if (isset($vars['node']) && arg(2) != 'edit' && $vars['node']->type == 'chat' && isset($_GET['chatroom_popout'])) {
    if ($vars['node']->chat->when_archived === NULL) {
      $vars['template_files'] = array('page-chat-popout');
    }
  }
}

/**
 * Theme the teaser for a chat node.
 *
 * @param mixed $node
 * @return string
 */
function theme_chatroom_chat_teaser($node) {
  if ($node->chat->when_archived) {
    $params = array(
      '!date' => format_date($node->chat->when_archived, 'medium'),
      '%message_count', $node->chat->msg_count,
    );
    return t('Archived on !date, contains %message_count messages.', $params);
  }
  else {
    if ($node->chat->latest_msg) {
      $params = array(
        '!date' => format_date($node->chat->latest_msg->modified, 'medium'),
        '%message' => $node->chat->latest_msg->msg,
        '!user' => theme('username', user_load($node->chat->latest_msg->uid)),
        '%message_count' => $node->chat->msg_count,
      );
      return t("Last message at !date: %message by !user. Total messages: %message_count.", $params);
    }
    else {
      return t('There are no messages in this chat.');
    }
  }
}

/**
 * Theme the teaser for a chatroom node.
 *
 * @param mixed $node
 * @return string
 */
function theme_chatroom_teaser($node) {
  $params = array(
    '%open_chats' => count($node->chatroom->chats),
  );
  return t('There are %open_chats chats in this chatroom.', $params);
}

/**
 * Theme a chatroom user list link.
 *
 * @param mixed $link
 * @return string
 */
function theme_chatroom_chat_user_link($link) {
  return '[' . l($link->text, $link->path, $link->options) . ']';
}

/**
 * Get HTML for max users message.
 *
 * @ingroup themeable
 */
function theme_chatroom_chat_max_users($node) {
  $msg = t('The threshold for maximum users has been reached for this chat, please try again in a few minutes.');
  return '<div id="chatroom-banned-msg">'. $msg .'</div>';
}

/**
 * Theme chatroom chat admin buttons.
 *
 * @param mixed $chat
 * @return string
 */
function theme_chatroom_chat_admin_buttons($chat) {
  return '<div class="chatroom-chat-admin-buttons">' . drupal_get_form('chatroom_chat_admin_buttons_form_' . $chat->nid, $chat) . '</div>';
}

/**
 * Format a username in a chatroom message.
 *
 * @param mixed $message
 * @param mixed $skip_permission_check
 * @return string
 */
function theme_chatroom_message_username($message, $skip_permission_check = FALSE) {
  if ($skip_permission_check || user_access('access user profiles')) {
    if ($message->uid > 0) {
      return l($message->name, 'user/'. $message->uid, array('attributes' => array('title' => t('View user profile.'))));
    }
    else {
      if ($message->anon_name) {
        return check_plain($message->anon_name) . ' <span class="anon-label">(' . variable_get('chatroom_guest_user_prefix', t('guest-')) . $message->guest_id . ')</span>';
      }
      else {
        return check_plain($message->name);
      }
    }
  }
  else {
    return check_plain($message->name);
  }
}


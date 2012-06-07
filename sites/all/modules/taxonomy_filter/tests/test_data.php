<?php
// $Id$

/**
 * @file
 * Create test data for Taxonomy Filter module.
 */

/**
 * Populate database with taxonomy (vocabularies and terms) and content.
 *
 * @todo Add term_relation data!!!
 */
function taxonomy_filter_add_test_data() {
  $version6 = function_exists('module_load_include');

//  file_put_contents('output.html', "starting\n", FILE_APPEND);

  $sql = "TRUNCATE {vocabulary}";
  $result = db_query($sql);
  if ($result === FALSE) {
//    file_put_contents('output.html', "messed up", FILE_APPEND);
  }
//  file_put_contents('output.html', "deleted vocabularies\n", FILE_APPEND);

  $relations = 1;
  $hierarchy = 1;
  $multiple = 1;
  $required = 0;
  $tags = 0;
  $module = 'taxonomy';

  $sql = "INSERT INTO {vocabulary} (vid, name, relations, hierarchy, multiple, required, tags, module, weight)
          VALUES (%d, '%s', %d, %d, %d, %d, %d, '%s', %d)";

  $i = 0;
  while ($i < 3) {
    $i++;
    $vid = $i;
    $name = 'Vocab #' . $i;
    $weight = $vid;
    $result = db_query($sql, $vid, $name, $relations, $hierarchy, $multiple, $required, $tags, $module, $weight);
    if ($result === FALSE) {
//      file_put_contents('output.html', "messed up vocabulary\n", FILE_APPEND);
    }
    else {
  //    file_put_contents('output.html', "inserted vocabulary $i\n", FILE_APPEND);
  //    file_put_contents('output.html', "$sql\n", FILE_APPEND);
    }
  }

  ///////////////////////////////////////

  $sql = "TRUNCATE {vocabulary_node_types}";
  $result = db_query($sql);
  if ($result === FALSE) {
//    file_put_contents('output.html', "messed up", FILE_APPEND);
  }
//  file_put_contents('output.html', "deleted vocabulary_node_types\n", FILE_APPEND);

  $type = 'story';

  $sql = "INSERT INTO {vocabulary_node_types} (vid, type)
          VALUES (%d, '%s')";

  $i = 0;
  while ($i < 3) {
    $i++;
    $vid = $i;
    $result = db_query($sql, $vid, $type);
    if ($result === FALSE) {
//      file_put_contents('output.html', "messed up vocabulary_node_types\n", FILE_APPEND);
    }
    else {
  //    file_put_contents('output.html', "inserted vocabulary_node_types $i\n", FILE_APPEND);
  //    file_put_contents('output.html', "$sql\n", FILE_APPEND);
    }
  }

  ///////////////////////////////////////

  $sql = "TRUNCATE {node}";
  $result = db_query($sql);
  if ($result === FALSE) {
//    file_put_contents('output.html', "messed up", FILE_APPEND);
  }
//  file_put_contents('output.html', "deleted nodes\n", FILE_APPEND);

  $type = 'story';
  $uid = 1;
  $status = 1;
  $created = 1242224668;
  $changed = 1242224668;
  $comment = 2;
  $promote = 1;

  $sql = "INSERT INTO {node} (nid, vid, type, title, uid, status, created, changed, comment, promote)
          VALUES (%d, %d, '%s', '%s', %d, %d, %d, %d, %d, %d)";

  $i = 0;
  while ($i < 125) {
    $i++;
    $nid = $i;
    $vid = $nid;
    $title = 'Story #' . $i;
    $result = db_query($sql, $nid, $vid, $type, $title, $uid, $status, $created, $changed, $comment, $promote);
    if ($result === FALSE) {
//      file_put_contents('output.html', "messed up node\n", FILE_APPEND);
    }
    else {
  //    file_put_contents('output.html', "inserted node $i\n", FILE_APPEND);
  //    file_put_contents('output.html', "$sql\n", FILE_APPEND);
    }
  }

  ///////////////////////////////////////

  $sql = "TRUNCATE {node_revisions}";
  $result = db_query($sql);

  $sql = "INSERT INTO {node_revisions} (nid, vid, uid, title, body, teaser, log, timestamp, format)
  SELECT nid, vid, uid, title, CONCAT('Content #', nid), CONCAT('Content #', nid), '', 1242224668, 1
  FROM node
  WHERE type = 'story'";
  $result = db_query($sql);

  $sql = "TRUNCATE {node_comment_statistics}";
  $result = db_query($sql);

  $sql = "INSERT INTO {node_comment_statistics} (nid, last_comment_timestamp, last_comment_name, last_comment_uid, comment_count)
  SELECT nid, 1242224668, NULL, 1, 0
  FROM node";
  $result = db_query($sql);

  ///////////////////////////////////////

  $sql = "TRUNCATE {term_data}";
  $result = db_query($sql);

  $sql = "INSERT INTO {term_data} (tid, vid, name, description, weight)
          VALUES (%d, %d, '%s', '%s', %d)";

  $i = 0;
  while ($i < 45) {
    $i++;
    $tid = $i;
    if ($tid < 11) {
      $vid = 1;
    }
    elseif ($tid < 21) {
      $vid = 2;
    }
    elseif ($tid < 31) {
      $vid = 3;
    }
    elseif ($tid < 41) {
      $vid = 1;
    }
    else {
      $vid = 2;
    }
    $name = 'term #' . $i;
    $description = 'description #' . $i;
    $weight = $i;

    $result = db_query($sql, $tid, $vid, $name, $description, $weight);
    if ($result === FALSE) {
//      file_put_contents('output.html', "messed up term_data\n", FILE_APPEND);
    }
    else {
  //    file_put_contents('output.html', "inserted term_data $i\n", FILE_APPEND);
  //    file_put_contents('output.html', "$sql\n", FILE_APPEND);
    }

  }

  ///////////////////////////////////////

  $sql = "TRUNCATE {term_hierarchy}";
  $result = db_query($sql);

  $sql = "INSERT INTO {term_hierarchy} (tid, parent)
          VALUES (%d, %d)";

  $i = 0;
  while ($i < 45) {
    $i++;
    $tid = $i;
    $parent = ($tid % 5) == 1 ? 0 : $tid - 1;

    $result = db_query($sql, $tid, $parent);
    if ($result === FALSE) {
//      file_put_contents('output.html', "messed up term_hierarchy\n", FILE_APPEND);
    }
    else {
  //    file_put_contents('output.html', "inserted term_hierarchy $i\n", FILE_APPEND);
  //    file_put_contents('output.html', "$sql\n", FILE_APPEND);
    }

  }

  ///////////////////////////////////////

  // Add term_relation!!!

  ///////////////////////////////////////

  $sql = "TRUNCATE {term_node}";
  $result = db_query($sql);

  if ($version6) {
    $sql = "INSERT INTO {term_node} (nid, vid, tid)
            VALUES (%d, %d, %d)";
  }
  else {
    $sql = "INSERT INTO {term_node} (nid, tid)
            VALUES (%d, %d)";
  }

  $i = 0;
  while ($i < 125) {
    $i++;
    $nid = $i;
    $vid = $nid;

    $first_tid = ($nid - 1) % 25 + 1;
    $j = 0;
    while ($j < 5) {
      $tid = $first_tid + 5 * $j;

      if ($version6) {
        $result = db_query($sql, $nid, $vid, $tid);
      }
      else {
        $result = db_query($sql, $nid, $tid);
      }
      if ($result === FALSE) {
//        file_put_contents('output.html', "messed up term_node\n", FILE_APPEND);
      }
      else {
  //      file_put_contents('output.html', "inserted term_node $i\n", FILE_APPEND);
  //      file_put_contents('output.html', "$sql\n", FILE_APPEND);
      }
      $j++;
    }

  }
//  file_put_contents('output.html', "done with changes\n", FILE_APPEND);
}

$(document).ready(function() {
	$("input[name=drupalchat_polling_method]").change(function() {
	    if ($("input[name=drupalchat_polling_method]:checked").val() == '0') {
	    	$('#edit-drupalchat-refresh-rate').removeAttr('disabled');
	    	$('#edit-drupalchat-send-rate').removeAttr('disabled');
	    	$('#edit-drupalchat-refresh-rate-wrapper').fadeIn();
	    	$('#edit-drupalchat-send-rate-wrapper').fadeIn();	    	
	    }
	    else if ($("input[@name=drupalchat_polling_method]:checked").val() == '1') {
	    	$('#edit-drupalchat-refresh-rate').attr('disabled', 'disabled');
	    	$('#edit-drupalchat-send-rate').attr('disabled', 'disabled');
	    	$('#edit-drupalchat-refresh-rate-wrapper').hide();
	    	$('#edit-drupalchat-send-rate-wrapper').hide();
	    }
	});

  $("input[name=drupalchat_rel]").change(function() {
      if ($("input[name=drupalchat_rel]:checked").val() == '1') {
        $('#edit-drupalchat-ur-name').removeAttr('disabled');
        $('#edit-drupalchat-ur-name-wrapper').fadeIn();     
      }
      else {
        $('#edit-drupalchat-ur-name').attr('disabled', 'disabled');
        $('#edit-drupalchat-ur-name-wrapper').hide();
      }
  });	
	
	$("input[name=drupalchat_polling_method]").change();
	$("input[name=drupalchat_rel]").change();
});


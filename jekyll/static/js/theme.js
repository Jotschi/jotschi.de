jQuery(document).ready(function() {
	jQuery('#comments').hide();
	jQuery("#access").sticky({
		topSpacing : 0,
		className : 'sticky',
		wrapperClassName : 'stickywrapper'
	});

	jQuery('#comment-form-toggle').click(function(event) {
		event.preventDefault();
		jQuery('#comments').slideToggle('slow');
	});

});
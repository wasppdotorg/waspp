var _ = function(json)
{
	if (json.status.code != 2000)
	{
		alert(json.status.message);
		return;
	}
	
	for (var sess in json.session)
	{
		var elems = document.getElementsByClassName('_' + sess);
		for (var i = 0; i < elems.length; ++i)
		{
			elems[i].appendChild(document.createTextNode(json.session[sess]));
		}
	}
};

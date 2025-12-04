function initUI()
{
	const buttons = document.getElementsByClassName("ui-button");
	for (let i = 0; i < buttons.length; i++) {
		console.log(buttons[i].id);
		buttons[i].onclick = function()
		{
			Module.ccall('OnButtonClick', null, ['string', 'int'], [buttons[i].id, 0]);
		}
	}
}
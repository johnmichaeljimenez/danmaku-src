function initUI()
{
	const buttons = document.getElementsByClassName("ui-button");
	for (let i = 0; i < buttons.length; i++) {
		// console.log(buttons[i].id);
		buttons[i].onclick = function()
		{
			Module.ccall('OnButtonClick', null, ['string', 'int'], [buttons[i].id, 0]);
		}
	}
}

function showUIScreen(id)
{
	const screens = document.getElementsByClassName("ui-screen");
	for (let i = 0; i < screens.length; i++) {
		screens[i].style.display = screens[i].id === id? 'block' : 'none';
	}
}
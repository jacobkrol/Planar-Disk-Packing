var openFile = function(event) {
    var input = event.target;
    var reader = new FileReader();
    reader.onload = function() {
        doc = reader.result;
        saveCircles();
    }
    reader.readAsText(input.files[0]);
}

function saveCircles() {
    doc = doc.replace(/\n/g,",");
    doc = doc.split(",");
	newSites = [];
	for( line of doc ) {
		line = line.replace(/\t/g, " ");
		line = line.split(" ");
		if( line == "" /* == intentional */) {
			continue;
		}
		var origPal = 500,
			tarPal = 500,
			scale = tarPal / origPal;
		point = {};
		point.x = Number(line[0])*scale;
		point.y = Number(line[1])*scale;
		point.weight = Number(line[2])*scale*wScale;
		newSites.push(point);
	}
	var weights = get_weights(newSites);
	for( point of newSites ) {
		point.index = newSites.indexOf(point);
		if(point.weight === weights[0]) {
			point.group = 'red';
		} else if(point.weight === weights[1]) {
			point.group = 'teal';
		} else {
			point.group = 'ERROR(1)';
		}
	}

	//hide old sites
	hide_sites();

	sites = newSites;
	weightUpdated();

	//correct cell color
	var cells = cellContainer.selectAll(".cell")._groups[0];
	var count = 0;
	for(var i=0; i<cells.length; i++) {
		var group = cells[i].__data__.site.originalObject.group;
		cells[i].classList.value = "group-"+group+" cell";
	}

	computeAllCells();
	redrawAllCells(WITHOUT_TRANSITION);

}

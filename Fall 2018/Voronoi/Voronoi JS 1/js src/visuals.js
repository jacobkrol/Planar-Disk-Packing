function rand_sites() {
	
	//hide old sites
	hide_sites();
	
	//weights
	var size1 = {min: 8, max: 12},
		size2 = {min: 4, max: 8},
		w1 = Math.floor(Math.random()*(size1.max-size1.min+1)+size1.min),
		w2 = Math.floor(Math.random()*(size2.max-size2.min+1)+size2.min);
	
	//randomize site data
	newSites = [];
	var i = 0,
		attempts = 0;
	while( 1 /*yeah yeah I know, save the lecture*/ ) {
		var disk = getNewDisk(w1, w2, 0.5);
		if(disk === null) { break; }
		disk.index = i++;
		newSites.push(disk);
	}

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


function stand_sites() {

	//hide old sites
	hide_sites();
	
	//take in new site data
	newSites = [
		{x:100,y:200,weight:10},
		{x:150,y:210,weight:15},
		{x:310,y:60,weight:10},
		{x:230,y:80,weight:10},
		{x:390,y:140,weight:15},
		{x:360,y:450,weight:15}
	];

	//add necessary data to cells
	for(var i=0; i<newSites.length; i++) {
		newSites[i].index = i;
		var weights = get_weights(newSites);
		if(newSites[i].weight == weights[0]) {
			newSites[i].group = "red";
		} else {
			newSites[i].group = "teal";
		}
		
	}

	sites = newSites;
	weightUpdated();
	
	//correct cell color
	var cells = cellContainer.selectAll(".cell")._groups[0];
	var count = 0;
	for(var i=0; i<cells.length; i++) {
		var group = cells[i].__data__.site.originalObject.group;
		cells[i].classList.value = "group-"+group+" cell";
	} 

}

function calc_verts() {
	
	//determine vertex points
	//start asynchronous loop for UI updates
	vertices = [];
	i = 0;
	addVerts();

}

function print_verts() {
	
	//print vertices to page
	document.getElementById("results").innerHTML = "";
	for(var i=0; i<vertices.length; i++) {
		document.getElementById("results").innerHTML += vertices[i][0]+","+vertices[i][1]+"<br>";
	}

	//count finite vertices
	var count = 0;
	for(var i=0; i<vertices.length; i++) {
		if(vertices[i][0] != 0 && vertices[i][1] != 0 && 
		vertices[i][0] != 500 && vertices[i][1] != 500) {
			count++;
		}
	}
	
	//clear 'printing' notification
	document.getElementById("results-title").innerHTML = "";
	
}

function addVerts() {
	
	//pull cell data
	var cells = weightedVoronoi(sites);
	
	//progress bar
	var prog = String(100 * (i+1) / cells.length)+"%";
	document.getElementsByClassName("progress")[0].style.width = prog;
	
	//pull vertex values
	for( vertex of cells[i] ) {
		vertex[0] = closeTo(vertex[0], 500) ? 500 : Number(vertex[0]);
		vertex[0] = closeTo(vertex[0], 0  ) ? 0   : Number(vertex[0]);
		vertex[1] = closeTo(vertex[1], 500) ? 500 : Number(vertex[1]);
		vertex[1] = closeTo(vertex[1], 0  ) ? 0   : Number(vertex[1]);
		var index = searchForPoints(vertices, vertex);
		if( index === -1 ) {
			vertex.push([cells[i].site]);
			vertices.push(vertex);
		} else {
			vertices[index][2].push(cells[i].site);
		}
	}
	
	//update loop
	if( ++i < cells.length ) {
		setTimeout(addVerts, 0);
	} else {
		document.getElementById("results-title").innerHTML = "Printing...";
		setTimeout(print_verts, 0);
	}
	
}

function show_sites(rsites) { //pass in 'real sites'

	//determine 2 weights
	var weights = get_weights(rsites);
	
	//delete extra g elements
	//FOR SPEED
	drawnSites._groups[0].splice(1);
	
	//delete extra seed elements
	//FOR CLEANLINESS AND MEMORY (?)
	var seeds = siteContainer.selectAll(".seed")._groups[0];
	for( var i=1; i<seeds.length; i++ ) {
		seeds[i].parentNode.removeChild(seeds[i]);
	}
	
	for(var i=0; i<rsites.length; i++) {
		//add sites to container
		drawnSites.append("circle")
        .attr("id", "seed-r-"+i)
        .attr("class", "group-"+rsites[i].group)
        .classed("seed", true)
        .attr("r", rsites[i]['weight']/wScale)
      	.attr("opacity", 0)
      	.attr("transform", "translate("+rsites[i].x+","+rsites[i].y+")");
		
		//tag them by their group
		siteContainer.selectAll(".seed")._groups[0][1+i].__data__.group = rsites[i].group
	}
	
	//remove first node (left at first to append to)
	seeds[0].parentNode.removeChild(seeds[0]);
	
	//draw sites
	redrawGroup("red",1,WITH_TRANSITION);
	redrawGroup("teal",1,WITH_TRANSITION);
	
}

function get_weights(rsites) {
	var w1 = rsites[0].weight,
		w2,
		i=0;
	while(++i < rsites.length) { 
		if(rsites[i].weight !== w1) {
			w2 = rsites[i].weight;
			break;
		} else {
			w2 = w1;
		}
	}
	return [w1,w2];
}

function hide_sites() {
	redrawGroup("red",0,WITH_TRANSITION);
	redrawGroup("teal",0,WITH_TRANSITION);
	redrawGroup("vertex",0,WITH_TRANSITION);
}

function open_verts() {
	avail = findFreeVerts(); //returns the vertices which could fit a disk
	
	//sets colors to correct weights
	var weights = get_weights(sites),
		maxW = Math.max(weights[0], weights[1]),
		minW = Math.min(weights[0], weights[1]);
	if(maxW === weights[0]) {
		var maxColor = "red",
			minColor = "teal";
	} else {
		var maxColor = "teal",
			minColor = "red";
	}
	
	//add the available vertices
	var orig = siteContainer.selectAll(".seed")._groups[0].length;
	for( var i=0; i<avail.length; i++) {
		var color = avail[i][1] ? maxColor : minColor;
		drawnSites.append("circle")
		.attr("id", "seed-v-"+i)
		.attr("class", "group-vertex fit-"+color)
		.classed("seed", true)
		.attr("r", 3)
		.attr("opacity", 0)
		.attr("transform", "translate("+avail[i][0][0]+","+avail[i][0][1]+")");
		
		//tag them by their group
		var curSites = siteContainer.selectAll(".seed")._groups[0];
		curSites[curSites.length-1].__data__.group = "vertex";
	}
	
	//draw them
	redrawGroup("vertex",1,WITH_TRANSITION);
}
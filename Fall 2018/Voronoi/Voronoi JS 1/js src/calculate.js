function getNewDisk(w1, w2, p) {
	var disk = {};
	
	if(Math.random() < p) {
		disk.weight = w1*wScale;
		disk.group = 'red';
	} else {
		disk.weight = w2*wScale;
		disk.group = 'teal';
	}
	var attempts = 0,
		tolerance = 500;
	do {
		disk.x = 500*Math.random();
		disk.y = 500*Math.random();
	} while( inside(disk) && attempts++ < tolerance );
	if(attempts > tolerance) {
		return null;
	}
	
	return disk;
}

function dist( [x1,y1], [x2,y2] ) { // returns distance between two points
	return Math.sqrt( Math.pow(x1-x2,2) + Math.pow(y1-y2,2) );
}

function closeTo( x, y ) {
	return Math.abs(x-y) < 1e-10;
}

function searchForPoints(haystack, needle){
  for(var i = 0; i < haystack.length; ++i){
    if( closeTo(needle[0], haystack[i][0]) && closeTo(needle[1], haystack[i][1]) ) {
		return i;
	}
  }
  return -1;
}

function inside(disk) { // returns TRUE if the disk is inside any others
	for( each of newSites ) {
		if( dist( [disk.x, disk.y], [each.x, each.y] ) < 
			disk.weight/wScale + each.weight/wScale ) {
			return true;
		}
	}
	return false;
}

function density(rsites) {
	var diskArea = 0,
		palArea = 500*500;
	for( disk of rsites ) {
		diskArea += Math.PI * Math.pow(disk.weight/wScale,2);
	}
	console.log(diskArea/palArea);
	return diskArea / palArea;
}

function findFreeVerts() {
	//update vertices
	var cells = weightedVoronoi(sites);
	var vertices = [];
	for( each of cells ) {
		for( vertex of each ) {
			vertex[0] = closeTo(vertex[0], 500) ? 500 : Number(vertex[0]);
			vertex[0] = closeTo(vertex[0], 0  ) ? 0   : Number(vertex[0]);
			vertex[1] = closeTo(vertex[1], 500) ? 500 : Number(vertex[1]);
			vertex[1] = closeTo(vertex[1], 0  ) ? 0   : Number(vertex[1]);
			var index = searchForPoints(vertices, vertex);
			if( index === -1 ) {
				vertex.push([each.site]);
				vertices.push(vertex);
			} else {
				vertices[index][2].push(each.site);
			}
		}
	}
	
	var avail = [];
	//find free vertices
	for( vertex of vertices ) {
		var weights = get_weights(sites),
			valid = true,
			validLarge = true,
			minW = Math.min(weights[0], weights[1]),
			maxW = Math.max(weights[0], weights[1]);
		for( site of vertex[2] ) {
			var space = dist( [vertex[0],vertex[1]], [site.x,site.y] );
			if( space < site.weight/wScale+maxW/wScale) {
				validLarge = false;
			}
			if( space < site.weight/wScale+minW/wScale ) {
				valid = false;
				break;
			}
		}
		
		if(validLarge) {
			avail.push( [vertex, 1] );
		} else if(valid) {
			avail.push( [vertex, 0] );
		}
	}
	
	return avail;
}
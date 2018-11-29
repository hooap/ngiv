


def is_firmus(first,second):
	### we dont know if its (lower-left & upper-right)  or  (upper-left & lower-right) 

	###convert to upperleft lowerright
	firstupperleft = []
	firstlowerright = []



	###get individual corners of the each object
	if(first[0] < first[2]):
		### first x is bigger than second
		firstupperleft.append(first[0])
		firstlowerright.append(first[2])
	else:
		firstupperleft.append(first[2])
		firstlowerright.append(first[0])

	if(first[1] > first[3]):
		###first dots y is bigger so its (upper-left & lower-right) for this object
		firstupperleft.append(first[1])
		firstlowerright.append(first[3])
	else:
		###second dots y is bigger so its (lower-left & upper-right) for this object
		firstupperleft.append(first[3])
		firstlowerright.append(first[1])




	###we got first obj's upperleft and lowerright
	### do it for second obj again
	secondupperleft = []
	secondlowerright = []

	if(second[0] < second[2]):
		### first x is bigger than second
		secondupperleft.append(second[0])
		secondlowerright.append(second[2])
	else:
		secondupperleft.append(second[2])
		secondlowerright.append(second[0])

	if(second[1] > second[3]):
		###first dots y is bigger so its (upper-left & lower-right) for this object
		secondupperleft.append(second[1])
		secondlowerright.append(second[3])
	else:
		###second dots y is bigger so its (lower-left & upper-right) for this object
		secondupperleft.append(second[3])
		secondlowerright.append(second[1])
	### we got upperleft and lowerright corners



	### check which one is the lower block 
	firstoneislower = False
	if(firstlowerright[1] < secondlowerright[1]):
		firstoneislower = True




### check if first rule is correct
	first_rule_correct = False
	if firstoneislower:
		if firstlowerright[1] == 0:
			first_rule_correct = True
	else:
		if secondlowerright[1] == 0:
			first_rule_correct = True 

###check if second rule is correct
	second_rule_correct = False

	bx1 = -1
	bx2 = -1
	by = -1

	ux1 = -1
	ux2 = -1
	uy = -1

	if firstoneislower:
		bx1 = firstupperleft[0]
		bx2 = firstlowerright[0]
		by = firstupperleft[1]

		ux1 = secondupperleft[0]
		ux2 = secondlowerright[0]
		uy = secondlowerright[1]
	else:
		bx1 = secondupperleft[0]
		bx2 = secondlowerright[0]
		by = secondupperleft[1]

		ux1 = firstupperleft[0]
		ux2 = firstlowerright[0]
		uy = firstlowerright[1]

	if(uy == by):
		###uppers bottom and lowers up corner is on the same y value
		if (ux2 > bx1 and bx1 < ux2) or (bx2 > ux1 and bx2 > ux1):
			### they make contact
			second_rule_correct = True


	centerofmassofupperobj = []
###get centerofmass of upperobj for third rule
	if firstoneislower:
		### add the x of centerofmass
		centerofmassofupperobj.append( ((float)(secondlowerright[0])+secondupperleft[0]) / 2.0)
		### add the y of centerofmass
		centerofmassofupperobj.append( ((float)(secondupperleft[1])+secondlowerright[1]) / 2.0)
	else:
		### add the x of centerofmass
		centerofmassofupperobj.append( ((float)(firstlowerright[0])+firstupperleft[0]) / 2.0)
		### add the y of centerofmass
		centerofmassofupperobj.append( ((float)(firstupperleft[1])+firstlowerright[1]) / 2.0)



###check if third rule is correct
	third_rule_correct = False
	if firstoneislower:
		if(centerofmassofupperobj[0] >= firstupperleft[0] and centerofmassofupperobj[0] <= firstlowerright[0]):
			third_rule_correct = True
	else:
		if(centerofmassofupperobj[0] >= secondupperleft[0] and centerofmassofupperobj[0] <= secondlowerright[0]):
			third_rule_correct = True
	

	### we got all of the rules 


### first output case
	if first_rule_correct and second_rule_correct and third_rule_correct:
		###its FIRMUS
		##lets calculate the area of objects		
		fxdif = firstlowerright[0] - firstupperleft[0]
		fydif = firstupperleft[1] - firstlowerright[1]
		farea = (fxdif * fydif)		

		sxdif = secondlowerright[0] - secondupperleft[0]
		sydif = secondupperleft[1] - secondlowerright[1]
		sarea = (sxdif * sydif)		

		output = ["FIRMUS",farea + sarea]
		return output

### second output case
	if(first_rule_correct and second_rule_correct):
		pass

###last output case
	



if __name__  == "__main__":
	print(is_firmus([0.5,19,9.5,9],[3.8,9,5.5,0]))
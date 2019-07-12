class 'MenuItem'

function MenuItem:__init(text, onSelect)
	self.text = text
	self.onSelect = onSelect
end

function MenuItem:OnSelect()
	self.onSelect()
end
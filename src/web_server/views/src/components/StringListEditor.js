import React, { useState, useEffect } from "react";

const StringListEditor = ({ list, setList, label, placeholder, itemName }) => {
  const [newItem, setNewItem] = useState("");

  useEffect(() => {
    console.log("list updated:", list);
  }, [list]);

  const handleDelete = (index) => {
    const updatedlist = list.filter((_, i) => i !== index);
    setList(updatedlist);
  };

    const handleAddSLE = async () => {
        const item = newItem.trim();
        if (item !== "") {
            setList([...list, item]);
            setNewItem("");
        }
    };

  return (
    <div className="stringlist-editor">
      <label>{label}</label>
      <ul>
        {list.map((str, index) => (
          <li key={index}>
            {str} 
            <button type="button" onClick={() => handleDelete(index)}>Delete</button>
          </li>
        ))}
      </ul>
      <div>
        <input
          type="text"
          placeholder={placeholder}
          value={newItem}
          onChange={(e) => setNewItem(e.target.value)}
          onKeyPress={ (e) => {
            if (e.key === "Enter") {
                e.preventDefault();
                handleAddSLE();
            }
            }}
        />
        <button type="button" onClick={handleAddSLE}>Add {itemName}</button>
      </div>
    </div>
  );
};

export default StringListEditor;
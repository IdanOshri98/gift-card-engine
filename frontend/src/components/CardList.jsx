import { useState } from "react";
import { deleteCard } from "../api";

export default function CardList({ cards, onDelete, onEdit }) {
  const [confirmId, setConfirmId] = useState(null);

  async function handleDelete(id) {
    if (confirmId === id) {
      await deleteCard(id);
      onDelete();
      setConfirmId(null);
    } else {
      setConfirmId(id);
    }
  }

  if (cards.length === 0) {
    return <p className="empty">No cards yet. Add your first card!</p>;
  }

  return (
    <table className="card-table">
      <thead>
        <tr>
          <th>Title</th>
          <th>Balance</th>
          <th>Expiry Date</th>
          <th>Days Left</th>
          <th>Companies</th>
          <th>Actions</th>
        </tr>
      </thead>
      <tbody>
        {cards.map((card) => (
          <tr
            key={card.id}
            className={
              card.daysLeft < 0
                ? "expired"
                : card.daysLeft <= 30
                ? "expiring-soon"
                : ""
            }
          >
            <td>{card.title}</td>
            <td>₪{card.balance.toFixed(2)}</td>
            <td>{card.expiryDate}</td>
            <td>{card.daysLeft < 0 ? "Expired" : card.daysLeft}</td>
            <td>{card.companies.join(", ")}</td>
            <td className="actions">
              <button onClick={() => onEdit(card)}>Edit</button>
              <button
                className={confirmId === card.id ? "confirm-delete" : "delete"}
                onClick={() => handleDelete(card.id)}
              >
                {confirmId === card.id ? "Sure?" : "Delete"}
              </button>
            </td>
          </tr>
        ))}
      </tbody>
    </table>
  );
}

import { useState } from "react";
import { updateCard } from "../api";

export default function EditCard({ card, onSave, onCancel }) {
  const [form, setForm] = useState({
    title: card.title,
    balance: card.balance,
    expiryDate: card.expiryDate,
    companies: card.companies.join(", "),
  });
  const [error, setError] = useState("");

  function handleChange(e) {
    setForm({ ...form, [e.target.name]: e.target.value });
  }

  async function handleSubmit(e) {
    e.preventDefault();
    setError("");

    if (Number(form.balance) <= 0) {
      setError("Balance must be greater than 0.");
      return;
    }

    const companies = form.companies
      ? form.companies.split(",").map((c) => c.trim()).filter(Boolean)
      : [];

    const result = await updateCard(card.id, {
      title: form.title,
      balance: Number(form.balance),
      expiryDate: form.expiryDate,
      companies,
    });

    if (result.error) {
      setError(result.error);
      return;
    }

    onSave();
  }

  return (
    <form className="card-form" onSubmit={handleSubmit}>
      <h2>Edit Card #{card.id}</h2>
      {error && <p className="error">{error}</p>}
      <input name="title" placeholder="Title" value={form.title} onChange={handleChange} />
      <input name="balance" type="number" placeholder="Balance" value={form.balance} onChange={handleChange} />
      <input name="expiryDate" placeholder="Expiry Date (DD-MM-YYYY)" value={form.expiryDate} onChange={handleChange} />
      <input name="companies" placeholder="Companies (comma separated)" value={form.companies} onChange={handleChange} />
      <div className="form-actions">
        <button type="submit">Save</button>
        <button type="button" onClick={onCancel}>Cancel</button>
      </div>
    </form>
  );
}

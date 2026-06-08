import { useState } from "react";
import { addCard } from "../api";

export default function AddCard({ onAdd }) {
  const [form, setForm] = useState({
    title: "",
    balance: "",
    expiryDate: "",
    companies: "",
  });
  const [error, setError] = useState("");

  function handleChange(e) {
    setForm({ ...form, [e.target.name]: e.target.value });
  }

  async function handleSubmit(e) {
    e.preventDefault();
    setError("");

    if (!form.title || !form.balance || !form.expiryDate) {
      setError("Title, balance and expiry date are required.");
      return;
    }
    if (Number(form.balance) <= 0) {
      setError("Balance must be greater than 0.");
      return;
    }

    const companies = form.companies
      ? form.companies.split(",").map((c) => c.trim()).filter(Boolean)
      : [];

    const result = await addCard({
      title: form.title,
      balance: Number(form.balance),
      expiryDate: form.expiryDate,
      companies,
    });

    if (result.error) {
      setError(result.error);
      return;
    }

    setForm({ title: "", balance: "", expiryDate: "", companies: "" });
    onAdd();
  }

  return (
    <form className="card-form" onSubmit={handleSubmit}>
      <h2>Add Card</h2>
      {error && <p className="error">{error}</p>}
      <input name="title" placeholder="Title" value={form.title} onChange={handleChange} />
      <input name="balance" type="number" placeholder="Balance" value={form.balance} onChange={handleChange} />
      <input name="expiryDate" placeholder="Expiry Date (DD-MM-YYYY)" value={form.expiryDate} onChange={handleChange} />
      <input name="companies" placeholder="Companies (comma separated)" value={form.companies} onChange={handleChange} />
      <button type="submit">Add Card</button>
    </form>
  );
}

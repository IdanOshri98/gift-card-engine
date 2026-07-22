import { useState } from "react";

export default function CardForm({ heading, initialValues, requireFields = false, submitLabel, onSubmit, onCancel }) {
  const [form, setForm] = useState(initialValues);
  const [error, setError] = useState("");

  function handleChange(e) {
    setForm({ ...form, [e.target.name]: e.target.value });
  }

  async function handleSubmit(e) {
    e.preventDefault();
    setError("");

    if (requireFields && (!form.title || !form.balance || !form.expiryDate)) {
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

    if (form.title.includes("|") || form.title.includes(",") || companies.some((c) => c.includes("|") || c.includes(","))) {
      setError("Title and company names can't contain '|' or ','");
      return;
    }

    try {
      await onSubmit({
        title: form.title,
        balance: Number(form.balance),
        expiryDate: form.expiryDate,
        companies,
      });
    } catch (err) {
      setError(err.message || "Network error — is the backend server running on port 8080?");
    }
  }

  return (
    <form className="card-form" onSubmit={handleSubmit}>
      <h2>{heading}</h2>
      {error && <p className="error">{error}</p>}
      <input name="title" placeholder="Title" value={form.title} onChange={handleChange} />
      <input name="balance" type="number" placeholder="Balance" value={form.balance} onChange={handleChange} />
      <input name="expiryDate" placeholder="Expiry Date (DD-MM-YYYY)" value={form.expiryDate} onChange={handleChange} />
      <input name="companies" placeholder="Companies (comma separated)" value={form.companies} onChange={handleChange} />
      {onCancel ? (
        <div className="form-actions">
          <button type="submit">{submitLabel}</button>
          <button type="button" onClick={onCancel}>Cancel</button>
        </div>
      ) : (
        <button type="submit">{submitLabel}</button>
      )}
    </form>
  );
}

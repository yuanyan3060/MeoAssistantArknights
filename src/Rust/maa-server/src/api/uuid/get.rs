use super::super::Error;
use crate::api::MaaManager;
use actix_web::{web, HttpResponse, Responder};
use serde::Deserialize;
use serde_json::json;
use std::sync::Mutex;

#[derive(Deserialize)]
pub struct Req {
    id: i64,
}
pub async fn get(
    req: web::Json<Req>,
    maa_manager: web::Data<Mutex<MaaManager>>,
) -> Result<impl Responder, Error> {
    let uuid = {
        let mut manager = maa_manager.lock().map_err(|_| Error::Internal)?;
        let maa = manager.get_mut(req.id).ok_or(Error::InstanceNotFound)?;
        maa.get_uuid()?
    };
    Ok(HttpResponse::Ok().json(json!({
        "uuid": uuid,
        "result":  0,
    })))
}

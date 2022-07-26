use super::super::{Error, MaaManager};
use actix_web::{web, HttpResponse, Responder};
use serde::Deserialize;
use serde_json::json;
use std::sync::Mutex;

#[allow(dead_code)]
#[derive(Deserialize)]
pub struct Req {
    id: i64,
}
pub async fn target(
    req: web::Json<Req>,
    maa_manager: web::Data<Mutex<MaaManager>>,
) -> Result<impl Responder, Error> {
    let target = {
        let manager = maa_manager.lock().map_err(|_| Error::Internal)?;
        manager.get(req.id).ok_or(Error::InstanceNotFound)?;
        manager.get_target(req.id)
    };
    Ok(HttpResponse::Ok().json(json!({
        "result":  0,
        "target": target
    })))
}
